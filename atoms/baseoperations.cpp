/*
Short Waves System - A numeric modular synthetizer
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

#include "atoms/baseoperations.h"
//static swsModuleRegistration<swsModuleValue>    swsModuleValueAddRegistration;
//static swsModuleRegistration<swsModuleAdd>      swsModuleAddRegistration;
//static swsModuleRegistration<swsModuleMultiply> swsModuleMultiplyRegistration;


swsModuleAdd::swsModuleAdd(): swsModule()
{
    mResult = newPlug("result", swsPlug::direction::output, 0);
    mOp1 = newPlug("op1", swsPlug::direction::input, 0);
    mOp2 = newPlug("op2", swsPlug::direction::input, 0);
};

void swsModuleAdd::step()
{
    mResult->setValue(mOp1->getValue() + mOp2->getValue());
}

swsModuleRegistration<swsModuleAdd> swsModuleAdd::mRegistration = swsModuleRegistration<swsModuleAdd>();
