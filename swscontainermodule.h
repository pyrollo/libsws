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

class swsContainerModule: public swsSchema, public swsModule::Registrar<swsContainerModule>
{
public:
    swsContainerModule(swsSchema *schema): swsSchema(), swsModule::Registrar<swsContainerModule>(schema) {}
    virtual ~swsContainerModule() {}
    static std::string getType() { return "container"; }

    void step() override {}
    swsSchema* toSchema() override { return this; }

};

#endif // SWSCONTAINERMODULE_H
