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

#ifndef SWSSCHEMA_H
#define SWSSCHEMA_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "swsmodule.h"

class swsSchema
{
public:
    swsSchema(swsSchema *parent);
    virtual ~swsSchema();

    swsSchema *parent() { return mParent; }

    swsSchema *getSchema(std::string schemaName);

    swsModule *getModule(std::string moduleName);
    virtual swsModule *newModule(std::string moduleName, std::string moduleType);
    void deleteModule(std::string moduleName);

    void expireSchedule() { mScheduled = false; }
    void step();

    std::unordered_map<std::string, swsModule *> const& modules() const { return mModules; }
    std::unordered_map<std::string, swsSchema *> const& schemas() const { return mSchemas; }

protected:
    swsSchema *mParent;
    std::unordered_map<std::string, swsSchema *> mSchemas;
    std::unordered_map<std::string, swsModule *> mModules;

    std::vector<swsModule *> mScheduledModules;
    bool mScheduled;

    bool isQueuable(swsModule *module, std::unordered_set<swsModule *> &unscheduledModules);
    void schedule();

    bool isAncestorOf(swsSchema *schema);
};

#endif // SWSSCHEMA_H
