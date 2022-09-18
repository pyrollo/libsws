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

#include "swsengine.h"

#include <sstream>
#include <vector>
#include <string>

#include "swscontainermodule.h"

void swsEngine::checkName(std::string name)
{
    if (name.find_first_not_of("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-") != std::string::npos)
        throw sws::invalid_name(name);
}

std::vector<std::string> swsEngine::splitPath(std::string path)
{
    std::stringstream stream(path);
    std::string atom;
    std::vector<std::string> result;
    while (std::getline(stream, atom, pathDelimiter))
      result.push_back(atom);

    return result;
}

std::string swsEngine::getBasePath(std::string path)
{
    auto pos = path.find_last_of(pathDelimiter);
    if (pos == std::string::npos)
        return "";
    else
        return path.substr(0, pos);
}

std::string swsEngine::getItemName(std::string path)
{
    std::string name;
    auto pos = path.find_last_of(pathDelimiter);
    if (pos == std::string::npos)
        name = path;
    else
        name = path.substr(pos+1);

    pos = name.find(attributeDelimiter);
    if (pos == std::string::npos)
        return name;
    else
        return name.substr(0, pos);
}

std::string swsEngine::getItemAttributeName(std::string path)
{
    std::string name;
    auto pos = path.find_last_of(pathDelimiter);
    if (pos == std::string::npos)
        name = path;
    else
        name = path.substr(pos+1);

    pos = name.find(attributeDelimiter);
    if (pos == std::string::npos)
        return "";
    else
        return name.substr(pos+1);
}

swsSchema *swsEngine::getSchema(std::string schemaPath)
{
    swsSchema *schema = &mRootSchema;

    std::vector<std::string> atoms = splitPath(schemaPath);
    for (auto atom: atoms) {
        schema = schema->getSchema(atom);
        if (!schema) {
            throw sws::unknown_schema(schemaPath);
        }
    }
    return schema;
}

swsModule *swsEngine::getModule(std::string modulePath)
{
    return getSchema(getBasePath(modulePath))->getModule(getItemName(modulePath));
}

swsPlug *swsEngine::getPlug(std::string plugPath)
{
    swsPlug *plug = getModule(plugPath)->plug(getItemAttributeName(plugPath));
    if (!plug)
        throw sws::unknown_plug(plugPath);
    return plug;
}


void swsEngine::newModule(std::string modulePath, std::string moduleType)
{
    getSchema(getBasePath(modulePath))->newModule(getItemName(modulePath), moduleType);
}

//void swsEngine::copyModule(std::string sourcePath, std::string targetPath);

void swsEngine::deleteModule(std::string modulePath)
{
    getSchema(getBasePath(modulePath))->deleteModule(getItemName(modulePath));
}

void swsEngine::connect(std::string plugPath1, std::string plugPath2)
{
    swsPlug *plug1 = getPlug(plugPath1);
    swsPlug *plug2 = getPlug(plugPath2);
    plug1->connect(plug2);
}

bool swsEngine::canConnect(std::string plugPath1, std::string plugPath2)
{
    swsPlug *plug1 = getPlug(plugPath1);
    swsPlug *plug2 = getPlug(plugPath2);
    return plug1->acceptConnection(plug2);
}

std::unordered_set<std::string> swsEngine::listConnectable(std::string plugPath)
{
    // TODO
    std::unordered_set<std::string> list;
    std::string schemaPath = getBasePath(plugPath);
    swsPlug *plug = getPlug(plugPath);
}

void swsEngine::disconnect(std::string plugPath1, std::string plugPath2)
{
    swsPlug *plug1 = getPlug(plugPath1);
    swsPlug *plug2 = getPlug(plugPath2);
    plug1->disconnect(plug2);
}


void swsEngine::set(std::string plugPath, swsValue value)
{
    getPlug(plugPath)->setValue(value);
}

swsValue swsEngine::get(std::string plugPath)
{
    return getPlug(plugPath)->getValue();
}

void swsEngine::step()
{
    mRootSchema.step();
}
