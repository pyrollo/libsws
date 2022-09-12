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

swsModule *swsEngine::getModule(std::string modulePath)
{
    return getSchema(getBasePath(modulePath))->getModule(getItemName(modulePath));
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

void swsEngine::newModule(std::string modulePath, std::string moduleType)
{
    getSchema(getBasePath(modulePath))->newModule(getItemName(modulePath), moduleType);
}

//void swsEngine::copyModule(std::string sourcePath, std::string targetPath);

void swsEngine::deleteModule(std::string modulePath)
{
    getSchema(getBasePath(modulePath))->deleteModule(getItemName(modulePath));
}

//void swsEngine::newPlug(std::string plugPath, std::string plugType);
//void swsEngine::deletePlug(std::string plugPath);
// --> est-ce que cela ne devrait pas être fait par la création de sous-modules spécifiques?

void swsEngine::connect(std::string sourcePlugPath, std::string targetPlugPath)
{
    swsModule *sourceModule = getModule(sourcePlugPath);
    swsModule *targetModule = getModule(targetPlugPath);

    swsPlug *sourcePlug = sourceModule->plug(getItemAttributeName(sourcePlugPath));
    if (!sourcePlug)
        throw sws::unknown_plug(sourcePlugPath);

    swsPlug *targetPlug = targetModule->plug(getItemAttributeName(targetPlugPath));
    if (!targetPlug)
        throw sws::unknown_plug(targetPlugPath);

    if (sourcePlug->getDirection() != swsPlug::direction::input ||
        targetPlug->getDirection() != swsPlug::direction::output)
        throw sws::illegal_connection();
}

//void swsEngine::canConnect(std::string sourcePlugPath, std::string targetPlugPath);
//void swsEngine::listConnectable(std::string plugPath);

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
