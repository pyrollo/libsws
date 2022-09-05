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

#include "swsplug.h"
#include "swsexception.h"

swsPlug::~swsPlug()
{
    // Disconnect
}

void swsPlug::connectTo(swsPlug *plug) {
    if (mDirection != direction::output)
        throw sws::illegal_connection();

    if (mConnectedTo.find(plug) != mConnectedTo.end())
        throw sws::already_connected();

    mConnectedTo.insert(plug);
}

void swsPlug::connectFrom(swsPlug *plug) {
    if (mDirection != direction::input)
        throw sws::illegal_connection();

    if (mConnectedFrom)
        throw sws::already_connected();

    mConnectedFrom = plug;
}

swsValue swsPlug::getValue() {
    if (mConnectedFrom)
        return mConnectedFrom->mValue;
    else
        return mValue;
}
