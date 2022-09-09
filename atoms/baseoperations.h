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

#ifndef BASEOPERATIONS_H
#define BASEOPERATIONS_H

#include <swsmodule.h>
#include <swsfactory.h>
#include <swsvalue.h>

class swsModuleValue: public swsModule::Registrar<swsModuleValue>
{
public:
    swsModuleValue(swsSchema *schema): swsModule::Registrar<swsModuleValue>(schema)
    {
        newPlug("value", swsPlug::direction::output, 0);
    }

    virtual ~swsModuleValue() {}

    static std::string getType() { return "value"; }

    void step() override
    {}
};

class swsModuleAdd : public swsModule::Registrar<swsModuleAdd>
{
public:
    swsModuleAdd(swsSchema *schema): swsModule::Registrar<swsModuleAdd>(schema)
    {
        mResult = newPlug("result", swsPlug::direction::output, 0);
        mOp1 = newPlug("op1", swsPlug::direction::input, 0);
        mOp2 = newPlug("op2", swsPlug::direction::input, 0);
    }

    virtual ~swsModuleAdd() {}

    static std::string getType() { return "add"; }

    void step() override
    {
        mResult->setValue(mOp1->getValue() + mOp2->getValue());
    }

private:
    swsPlug *mResult, *mOp1, *mOp2;
};

class swsModuleMultiply : public swsModule::Registrar<swsModuleMultiply>
{
public:
    swsModuleMultiply(swsSchema *schema): swsModule::Registrar<swsModuleMultiply>(schema)
    {
        mResult = newPlug("result", swsPlug::direction::output, 0);
        mOp1 = newPlug("op1", swsPlug::direction::input, 0);
        mOp2 = newPlug("op2", swsPlug::direction::input, 0);
    }

    virtual ~swsModuleMultiply() {}

    static std::string getType() { return "multiply"; }

    void step() override
    {
        mResult->setValue(mOp1->getValue() * mOp2->getValue());
    }

private:
    swsPlug *mResult, *mOp1, *mOp2;
};

#endif // BASEOPERATIONS_H
