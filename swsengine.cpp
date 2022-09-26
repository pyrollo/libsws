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

// Private stuff
// -------------

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


std::string swsEngine::pathJoin(std::string first, std::string last)
{
    if (last.empty())
        return first;

    if (first.empty())
        return last;

    return first + pathDelimiter + last;
}

std::string swsEngine::getPath(swsSchema *schema)
{
    if (schema == &mRootSchema)
        return "";

    if (!schema->parent())
        throw sws::internal_error("Non root schema with no parent");

    for (auto it: schema->parent()->schemas())
        if (it.second == schema)
            return pathJoin(getPath(schema->parent()), it.first);

    throw sws::internal_error("Schema not in its parent schemas");
}

std::string swsEngine::getPath(swsModule *module)
{
    for (auto it: module->getSchema()->modules())
        if (it.second == module)
            return pathJoin(getPath(module->getSchema()), it.first);

    throw sws::internal_error("Module not in its schema modules");
}

std::string swsEngine::getPath(swsPlug *plug)
{
    for (auto it: plug->getModule()->plugs())
        if (it.second == plug)
            return getPath(plug->getModule()) + attributeDelimiter + it.first;

    throw sws::internal_error("Plug not in its module plug list");
}

// Schema construction
// -------------------

void swsEngine::newModule(std::string modulePath, std::string moduleType)
{
    getSchema(getBasePath(modulePath))->newModule(getItemName(modulePath), moduleType);
}

void swsEngine::instantiateModule(std::string modulePath, std::string templatePath)
{
    getSchema(getBasePath(modulePath))->instantiateModule(getItemName(modulePath), getModule(templatePath));
}


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

void swsEngine::disconnect(std::string plugPath1, std::string plugPath2)
{
    swsPlug *plug1 = getPlug(plugPath1);
    swsPlug *plug2 = getPlug(plugPath2);
    plug1->disconnect(plug2);
}

// Access to values
// ----------------

void swsEngine::set(std::string plugPath, swsValue value)
{
    getPlug(plugPath)->setValue(value);
}

swsValue swsEngine::get(std::string plugPath)
{
    return getPlug(plugPath)->getValue();
}

// Execution
// ---------

void swsEngine::step()
{
    mRootSchema.step();
}

// Introspection
// -------------

std::unordered_set<std::string> swsEngine::listPlugs(std::string modulePath)
{
    std::unordered_set<std::string> result;

    swsModule *module = getModule(modulePath);
    for (auto it: module->plugs())
        result.insert(modulePath + attributeDelimiter + it.first);
    return result;
}

std::string swsEngine::getModuleType(std::string modulePath)
{
    return getModule(modulePath)->getType();
}

std::string swsEngine::getPlugType(std::string plugPath)
{
    switch(getPlug(plugPath)->getDirection()) {
    case swsPlug::direction::input:
        return "input";
        break;
    case swsPlug::direction::output:
        return "output";
        break;
    case swsPlug::direction::none:
        return "internal";
        break;
    }
    return "unknown";
}

bool swsEngine::canConnect(std::string plugPath1, std::string plugPath2)
{
    swsPlug *plug1 = getPlug(plugPath1);
    swsPlug *plug2 = getPlug(plugPath2);
    return plug1->acceptConnection(plug2);
}

std::unordered_set<std::string> swsEngine::listConnectable(std::string plugPath)
{
    std::unordered_set<swsPlug *> plugs = getPlug(plugPath)->listConnectable();
    std::unordered_set<std::string> list;
    for (swsPlug *plug: plugs)
        list.insert(getPath(plug));
    return list;
}
