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
#include <unordered_set>

#include "swsexception.h"
#include "swscontainermodule.h"

// These includes can be removed from sws.h to make them optional
    #include "atoms/baseoperations.h"

class swsEngine
{
public:
    const char pathDelimiter = '/';
    const char attributeDelimiter = '#';

    void newModule(std::string modulePath, std::string moduleType);
    void copyModule(std::string sourcePath, std::string targetPath);
    void deleteModule(std::string modulePath);

    void newPlug(std::string plugPath, std::string plugType);
    void deletePlug(std::string plugPath);

    void connect(std::string plugPath1, std::string plugPath2);
    bool canConnect(std::string plugPath1, std::string plugPath2);
    std::unordered_set<std::string> listConnectable(std::string plugPath);
    void disconnect(std::string plugPath1, std::string plugPath2);

    void set(std::string plugPath, swsValue value);
    swsValue get(std::string plugPath);

    void step();

private:
    swsSchema mRootSchema;

    void checkName(std::string name);
    std::vector<std::string> splitPath(std::string path);
    std::string getBasePath(std::string path);
    std::string getItemName(std::string path);
    std::string getItemAttributeName(std::string path);
    swsSchema *getSchema(std::string schemaPath);
    swsModule *getModule(std::string modulePath);
    swsPlug   *getPlug(std::string plugPath);
};

#endif // SWS_H
