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

#include "sws.h"

#include <sstream>
#include <vector>
#include <string>

#include "swscontainermodule.h"

namespace sws {

    static swsContainerModule root;

    void checkName(std::string name)
    {
        if (name.find_first_not_of("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-") != std::string::npos)
            throw invalid_name(name);
    }

    std::vector<std::string> splitPath(std::string path)
    {
        std::stringstream stream(path);
        std::string atom;
        std::vector<std::string> result;
        while (std::getline(stream, atom, pathDelimiter))
          result.push_back(atom);

        return result;
    }

    std::string getBasePath(std::string path)
    {
        auto pos = path.find_last_of(pathDelimiter);
        if (pos == std::string::npos)
            return "";
        else
            return path.substr(0, pos);
    }

    std::string getItemName(std::string path)
    {
        auto pos = path.find_last_of(pathDelimiter);
        if (pos == std::string::npos)
            return path;
        else
            return path.substr(pos+1);
    }

    swsModule *getModule(std::string modulePath)
    {
        swsModule *module = &root;
        swsContainer *container = module->toContainer();

        std::vector<std::string> atoms = splitPath(modulePath);

        for (auto atom: atoms) {
            if (!container)
                throw unknown_module(modulePath);

            module = container->getModule(atom);
            container = module->toContainer();
        }

        return module;
    }

    swsContainer *getContainer(std::string modulePath)
    {
        swsModule *module = getModule(getBasePath(modulePath));
        swsContainer *container = module->toContainer();

        if (!container)
            throw illegal_operation();

        return container;
    }

    void newModule(std::string modulePath, std::string moduleType)
    {
        getContainer(modulePath)->newModule(getItemName(modulePath), moduleType);
    }

//void copyModule(std::string sourcePath, std::string targetPath);

    void deleteModule(std::string modulePath)
    {
        getContainer(modulePath)->deleteModule(getItemName(modulePath));
    }

//void newPlug(std::string plugPath, std::string plugType);
//void deletePlug(std::string plugPath);
// --> est-ce que cela ne devrait pas être fait par la création de sous-modules spécifiques?

//void connect(std::string sourcePlugPath, std::string targetPlugPath);
//void canConnect(std::string sourcePlugPath, std::string targetPlugPath);
//void listConnectable(std::string plugPath);

}
