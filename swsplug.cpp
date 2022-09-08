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

swsPlug::~swsPlug()
{
    // Disconnect
}

swsValue swsPlug::getValue() {
    if (mConnectedFrom)
        return mConnectedFrom->mValue;
    else
        return mValue;
}

void swsPlug::testConnection(swsPlug *plug)
{
    switch (mDirection) {
    case direction::output:
        if (plug->getDirection() != direction::input)
            throw sws::illegal_connection();
        if (mConnectedTo.find(plug) != mConnectedTo.end())
            throw sws::already_connected();
        break;
    case direction::input:
        if (plug->getDirection() != direction::output)
            throw sws::illegal_connection();
        if (mConnectedFrom)
            throw sws::already_connected();
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
        plug->testConnection(this);
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
    plug->testConnection(this);

    switch (mDirection) {
    case direction::output:
        mConnectedTo.insert(plug);
        plug->mConnectedFrom = plug;
    case direction::input:
        mConnectedFrom = plug;
        plug->mConnectedTo.insert(this);
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
                plug->disconnect(this);
            mConnectedTo.empty();
        }
        if (plug->mConnectedFrom == this) {
            plug->disconnect(this);
            mConnectedTo.erase(plug);
        }
        break;
    case direction::input:
        if (!plug || mConnectedFrom == plug)
            mConnectedFrom->disconnect(this);
        break;
    default:
        break;
    }
}
