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

#include "swsmodule.h"
#include "swsexception.h"

#include <stdexcept>

swsModule::swsModule(swsSchema *schema): mSchema(schema)
{
}

swsModule::~swsModule()
{
    for (auto it: mPlugs)
        delete it.second;
}

swsPlug *swsModule::newPlug(std::string name, swsPlug::direction direction, swsValue value)
{
    if (mPlugs.find(name) != mPlugs.end())
        throw sws::duplicate_name();

    swsPlug *plug = new swsPlug(this, direction, value);
    mPlugs[name] = plug;
    return plug;
}

void swsModule::deletePlug(swsPlug *plug)
{
    if (plug->getModule() == this) {
        for (auto it = mPlugs.begin(); it != mPlugs.end(); ++it)
            if (it->second == plug) {
                delete it->second;
                mPlugs.erase(it);
                return;
            }

        throw sws::unknown_plug("(anonymous)");
    }
}

void swsModule::deleteAnyPlug(swsPlug *plug)
{
    plug->getModule()->deletePlug(plug);
}

swsPlug *swsModule::plug(std::string name) const
{
    try {
        return mPlugs.at(name);
    } catch(const std::out_of_range&) {
        throw sws::unknown_plug(name);
    }
}

void swsModule::listAddConnected(swsPlug::direction direction, std::unordered_set<swsModule *> &list)
{
    if (list.find(this) != list.end())
        return;

    if (isInterconnected()) {
        list.insert(this);
        for (auto it: mPlugs) {
            if (it.second->getDirection() != direction)
                continue;
            if (it.second->connectedFrom())
                it.second->connectedFrom()->getModule()->listAddConnected(direction, list);
            for (auto connected: it.second->connectedTo())
                connected->getModule()->listAddConnected(direction, list);
        }
    }

}
