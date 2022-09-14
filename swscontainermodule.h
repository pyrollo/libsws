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

#ifndef SWSCONTAINERMODULE_H
#define SWSCONTAINERMODULE_H

#include <map>
#include <vector>

#include "swsmodule.h"
#include "swsschema.h"

class swsContainerModule: public swsModule::Registrar<swsContainerModule>, public swsSchema
{
public:
    swsContainerModule(swsSchema *schema): swsModule::Registrar<swsContainerModule>(schema), swsSchema() {}
    virtual ~swsContainerModule() {}

    static std::string getType() { return "container"; }

    void step() override;

    swsSchema* toSchema() override { return this; }

    swsModule *newModule(std::string moduleName, std::string moduleType) override;
};

class swsInputModule: public swsModule::Registrar<swsInputModule>
{
    friend class swsContainerModule;

public:
    swsInputModule(swsSchema *schema);
    ~swsInputModule() override;

    static std::string getType() { return "input"; }

    void step() override;

private:
    swsPlug *mValue, *mExtern;
};

class swsOutputModule: public swsModule::Registrar<swsOutputModule>
{
    friend class swsContainerModule;

public:
    swsOutputModule(swsSchema *schema);
    virtual ~swsOutputModule();

    static std::string getType() { return "output"; }

    void step() override;

private:
    swsPlug *mValue, *mExtern;
};

#endif // SWSCONTAINERMODULE_H
