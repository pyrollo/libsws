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

#ifndef SWSMODULE_H
#define SWSMODULE_H

#include <string>
#include <map>

#include "swsplug.h"
#include "swsvalue.h"
#include "swsfactory.h"

class swsSchema;

typedef swsFactory<swsModule> swsModuleFactory;

class swsModule: public swsModuleFactory
{
public:
    swsModule();
    virtual ~swsModule();

    virtual void step() = 0;
    static std::string getType() { return "unknown"; }
    virtual bool isInterconnected() { return true; }

    swsPlug *newPlug(std::string name, swsPlug::direction direction, swsValue value);
    void deletePlug(std::string name);
    std::unordered_set<std::string> listPlugs();

    swsPlug *plug(std::string plugName) const;
    std::unordered_set<swsPlug *> plugs();

    virtual swsSchema* toSchema() { return nullptr; }

protected:
    std::map<std::string, swsPlug *> mPlugs;
    swsSchema *mSchema;
};

#endif // SWSMODULE_H
