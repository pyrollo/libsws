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

#include "swsschema.h"

#include <stdexcept>

#include "swsexception.h"


swsSchema::swsSchema(swsSchema *parent): mParent(parent), mScheduled(false)
{
}

swsSchema::~swsSchema()
{
    for (auto it: mModules)
        delete it.second;
}

swsModule *swsSchema::newModule(std::string moduleName, std::string moduleType)
{
    // TODO: mutex preventing step

    try {
        mModules.at(moduleName);
        throw sws::duplicate_name();
    } catch(const std::out_of_range&) {}

    swsModule *module = swsModuleFactory::produce(this, moduleType);
    mModules[moduleName] = module;
    expireSchedule();

    swsSchema *schema = module->toSchema();
    if (schema)
        mSchemas[moduleName] = schema;

    return module;
}

bool swsSchema::isDescendantOf(swsSchema *schema)
{
    if (schema == this)
        return true;

    if (mParent)
        return mParent->isDescendantOf(schema);

    return false;
}

swsModule *swsSchema::instantiateModule(std::string moduleName, swsModule *templateModule)
{
    swsSchema *templateSchema = templateModule->toSchema();

    // Cannot instanciate a module inside itself (inside itself (inside itself ...))
    if (templateSchema && isDescendantOf(templateSchema))
        throw sws::illegal_operation();

    swsModule *module = newModule(moduleName, templateModule->getType());

    // Basic module
    if (!templateSchema)
    {
        // Copy internal vars
        for (auto it: templateModule->plugs())
            module->plug(it.first)->setValue(it.second->getValue());

        return module;
    }

    // Complex module
    if (!module->toSchema())
        throw sws::internal_error("Module type " + templateModule->getType() + " is suposed to be complex but it's not");

    std::unordered_map<swsPlug *, swsPlug *> plugMap;

    for (auto itModule: templateSchema->modules()) {
        swsModule *newModule = module->toSchema()->instantiateModule(itModule.first, itModule.second);
        for (auto itPlug: itModule.second->plugs())
            plugMap[itPlug.second] = newModule->plug(itPlug.first);
    }

    // Copy internal vars (must be done after submodules instanciation)
    for (auto it: templateModule->plugs())
        module->plug(it.first)->setValue(it.second->getValue());

    // Connectics
    for (auto itModule: templateSchema->modules())
        for (auto itPlug: itModule.second->plugs())
            if (itPlug.second->connectedFrom())
                plugMap[itPlug.second]->connect(plugMap[itPlug.second->connectedFrom()]);

    return module;
}

swsSchema *swsSchema::getSchema(std::string schemaName)
{
    try {
        return mSchemas.at(schemaName);
    } catch(const std::out_of_range&) {
        throw sws::unknown_schema(schemaName);
    }
}

swsModule *swsSchema::getModule(std::string moduleName)
{
    try {
        return mModules.at(moduleName);
    } catch(const std::out_of_range&) {
        throw sws::unknown_module(moduleName);
    }
}

void swsSchema::deleteModule(std::string moduleName)
{
    // TODO: mutex preventing step

    auto it = mModules.find(moduleName);
    if (it == mModules.end())
        throw sws::unknown_module(moduleName);

    delete it->second;
    mModules.erase(it);
    expireSchedule();
}

bool swsSchema::isQueuable(swsModule *module, std::unordered_set<swsModule *> &unscheduledModules)
{
    // Check that all upstream modules are queued
    if (module->isInterconnected())
        for (auto it: module->plugs())
            if (it.second->connectedFrom() && unscheduledModules.find(it.second->connectedFrom()->getModule()) != unscheduledModules.end())
                return false;

    return true;
}

void swsSchema::schedule()
{
    std::unordered_set<swsModule *> unscheduledModules;

    for (auto it: mModules)
        unscheduledModules.insert(it.second);

    mScheduledModules.clear();

    bool change;
    do {
        change = false;
        for (auto module = unscheduledModules.begin(); module != unscheduledModules.end(); ) {
            if (isQueuable(*module, unscheduledModules)) {
                change = true;
                mScheduledModules.push_back(*module);
                module = unscheduledModules.erase(module);
            } else
                module++;
        }
    } while(change);

    if (unscheduledModules.size()) {
        throw sws::unable_to_schedule_children();
    }

    mScheduled = true;
}

void swsSchema::step()
{
    //const std::lock_guard<std::mutex> lock(mStepMutex);

    if (!mScheduled)
        schedule();

    for (auto module: mScheduledModules)
        module->step();
}
