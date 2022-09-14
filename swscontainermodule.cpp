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

#include "swscontainermodule.h"

swsModule *swsContainerModule::newModule(std::string moduleName, std::string moduleType)
{
    swsModule *module = swsSchema::newModule(moduleName, moduleType);

    if (moduleType == "input")
    {
        ((swsInputModule *)module)->mExtern = newPlug(moduleName, swsPlug::direction::input, 0);
    }

    if (moduleType == "output")
    {
        ((swsOutputModule *)module)->mExtern = newPlug(moduleName, swsPlug::direction::output, 0);
    }

    return module;
}

void swsContainerModule::step()
{
    swsSchema::step();
}

swsInputModule::swsInputModule(swsSchema *schema): swsModule::Registrar<swsInputModule>(schema), mExtern(nullptr)
{
    mValue = newPlug("value", swsPlug::direction::output, 0);
}

swsInputModule::~swsInputModule()
{
    if (mExtern)
        deleteAnyPlug(mExtern);
}

void swsInputModule::step()
{
    if (mExtern)
        mValue->setValue(mExtern->getValue());
}

swsOutputModule::swsOutputModule(swsSchema *schema): swsModule::Registrar<swsOutputModule>(schema), mExtern(nullptr)
{
    mValue = newPlug("value", swsPlug::direction::input, 0);
}

swsOutputModule::~swsOutputModule()
{
    if (mExtern)
        deleteAnyPlug(mExtern);
}

void swsOutputModule::step()
{
    if (mExtern)
        mExtern->setValue(mValue->getValue());
}
