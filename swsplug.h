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

#ifndef SWSPLUG_H
#define SWSPLUG_H

#include <unordered_set>

#include "swsvalue.h"

class swsModule;

class swsPlug
{
public:
    enum class direction {
        none,
        output,
        input
    };

    swsPlug(swsModule *module, direction direction, swsValue value):
        mModule(module), mDirection(direction), mValue(value), mConnectedFrom(nullptr) {}

    virtual ~swsPlug();

    direction getDirection() const { return mDirection; }
    swsModule *getModule() const { return mModule; }

    swsPlug *connectedFrom() { return mConnectedFrom; }

    swsValue getValue();
    void setValue(swsValue value) { mValue = value; }

    bool acceptConnection(swsPlug *plug);
    void connect(swsPlug *plug);
    void disconnect(swsPlug *plug = nullptr);

private:
    swsModule *mModule;
    direction mDirection;
    swsValue mValue; // TODO: Type of value could be chosen
    std::unordered_set <swsPlug *> mConnectedTo;
    swsPlug *mConnectedFrom;

    void testConnection(swsPlug *plug);
    bool isUpstream(swsPlug *plug);
};

#endif // SWSPLUG_H
