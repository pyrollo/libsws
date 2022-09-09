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

void swsModule::deletePlug(std::string name)
{
    if (mPlugs.find(name) == mPlugs.end())
        throw sws::unknown_plug(name);

    delete mPlugs[name];
    mPlugs.erase(name);
}

swsPlug *swsModule::plug(std::string name) const
{
    try {
        return mPlugs.at(name);
    } catch(const std::out_of_range&) {
        throw sws::unknown_plug(name);
    }
}

std::unordered_set<std::string> swsModule::listPlugs()
{
    std::unordered_set<std::string> plugs;

    for (auto it: mPlugs)
        plugs.insert(it.first);

    return plugs;
}
