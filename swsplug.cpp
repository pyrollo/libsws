/*
Short Waves System Library - A numeric modular synthetizer
Copyright (C) 2021 Pierre-Yves Rollo <dev@pyrollo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "swsplug.h"
#include "swsexception.h"
#include "swsschema.h"
#include "swsmodule.h"
#include "swsplug.h"

swsPlug::~swsPlug()
{
    disconnect();
}

swsValue swsPlug::getValue() {
    if (mConnectedFrom)
        return mConnectedFrom->mValue;
    else
        return mValue;
}

bool swsPlug::isUpstream(swsPlug *plug)
{
    if (plug == this)
        return true;

    if  (mDirection == direction::input) {
        if (!mConnectedFrom)
            return false;

        return mConnectedFrom->isUpstream(plug);
    }

    if (!mModule->isInterconnected())
        return false;

    if  (mDirection == direction::output) {
        for (auto it: mModule->plugs())
            if (it.second->mDirection == direction::input)
                if (it.second->isUpstream(plug))
                    return true;
    }

    return mDirection == direction::none;
}

void swsPlug::testConnection(swsPlug *plug)
{
    if (mModule->getSchema() != plug->mModule->getSchema())
        throw sws::illegal_connection();

    switch (mDirection) {
    case direction::output:
        if (plug->getDirection() != direction::input)
            throw sws::illegal_connection();
        if (mConnectedTo.find(plug) != mConnectedTo.end())
            throw sws::already_connected();
        if (isUpstream(plug))
            throw sws::illegal_connection();
        break;

    case direction::input:
        if (plug->getDirection() != direction::output)
            throw sws::illegal_connection();
        if (mConnectedFrom)
            throw sws::already_connected();
        if (plug->isUpstream(this))
            throw sws::illegal_connection();
        break;

    default:
        throw sws::illegal_connection();
        break;
    }
}

bool swsPlug::acceptConnection(swsPlug *plug)
{
    try {
        testConnection(plug);
    } catch (sws::already_connected) {
        return false;
    } catch (sws::illegal_connection) {
        return false;
    }
    return true;
}

void swsPlug::connect(swsPlug *plug)
{
    if (!plug)
        return;

    testConnection(plug);

    switch (mDirection) {
    case direction::output:
        mConnectedTo.insert(plug);
        plug->mConnectedFrom = this;
        break;
    case direction::input:
        mConnectedFrom = plug;
        plug->mConnectedTo.insert(this);
        break;
    default:
        break;
    }
}

void swsPlug::disconnect(swsPlug *plug)
{
    switch (mDirection) {
    case direction::output:
        if (!plug) {
            // Disconnect all
            for (auto plug: mConnectedTo)
                plug->mConnectedFrom = nullptr;
            mConnectedTo.empty();
            return;
        }
        if (plug->mConnectedFrom == this) {
            plug->mConnectedFrom = nullptr;
            mConnectedTo.erase(plug);
        }
        break;
    case direction::input:
        if (mConnectedFrom && (!plug || mConnectedFrom == plug))
            mConnectedFrom->disconnect(this);
        break;
    default:
        break;
    }
}

std::unordered_set<swsPlug *> swsPlug::listConnectable()
{
    std::unordered_set<swsPlug *> list;
    std::unordered_set<swsModule *> unconnectable;

    switch (mDirection) {
    case direction::none:
        break;
    case direction::input:
        if (mConnectedFrom)
            break;
        mModule->listAddConnected(swsPlug::direction::output, unconnectable);
        for (auto itModule: mModule->getSchema()->modules())
            if (unconnectable.find(itModule.second) == unconnectable.end())
                for (auto itPlug: itModule.second->plugs())
                    if (itPlug.second->mDirection == direction::output)
                        list.insert(itPlug.second);
        break;
    case direction::output:
        mModule->listAddConnected(swsPlug::direction::input, unconnectable);
        for (auto itModule: mModule->getSchema()->modules())
            if (unconnectable.find(itModule.second) == unconnectable.end())
                for (auto itPlug: itModule.second->plugs())
                    if (itPlug.second->mDirection == direction::input &&
                           !itPlug.second->mConnectedFrom)
                        list.insert(itPlug.second);
        break;
    }
    return list;
}
