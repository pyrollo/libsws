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

#ifndef SWSCONTAINER_H
#define SWSCONTAINER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "swsmodule.h"

class swsContainer
{
public:
    swsContainer();
    virtual ~swsContainer();

    swsModule *newModule(std::string moduleName, std::string moduleType);
    swsModule *instanciateModule(std::string moduleName, swsContainer *container);
    void deleteModule(std::string moduleName);

    std::unordered_set<std::string> listModules();
    swsModule *getModule(std::string moduleName);

    void expireSchedule() { mScheduled = false; }
    void step();

    swsContainer* toContainer() { return this; }

protected:
    std::unordered_map<std::string, swsModule *> mModules;
    std::vector<swsModule *> mScheduledModules;
    bool mScheduled;

    bool isQueuable(swsModule *module, std::unordered_set<swsModule *> &unscheduledModules);
    void schedule();

    bool isAncestorOf(swsContainer *container);
};

#endif // SWSCONTAINER_H
