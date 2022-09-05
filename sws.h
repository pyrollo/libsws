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

#ifndef SWS_H
#define SWS_H

#include <string>

#include "swsexception.h"

// These includes can be removed from sws.h to make them optional
    #include "atoms/baseoperations.h"

namespace sws {
    const char pathDelimiter = '/';

    void newModule(std::string modulePath, std::string moduleType);
    void copyModule(std::string sourcePath, std::string targetPath);
    void deleteModule(std::string modulePath);

    void newPlug(std::string plugPath, std::string plugType);
    void deletePlug(std::string plugPath);

    void connect(std::string sourcePlugPath, std::string targetPlugPath);
    void canConnect(std::string sourcePlugPath, std::string targetPlugPath);
    void listConnectable(std::string plugPath);
}

#endif // SWS_H
