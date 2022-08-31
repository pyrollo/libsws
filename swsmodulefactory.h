#ifndef SWSMODULEFACTORY_H
#define SWSMODULEFACTORY_H

#include <string>
#include <unordered_map>

#include "swsexception.h"

class swsModule;

class swsModuleFactory
{
public:
    template<typename T>
    static void registerModuleType(const std::string &typeName)
    {
        static_assert(std::is_base_of<swsModule, T>::value, "Cannot register a class that is not a swsModule");

        auto it = instanciators()->find(typeName);

        if (it != instanciators()->end())
            return;
            //throw sws::duplicate_name();

        instanciators()->emplace(typeName, &createInstance<T>);
    }

    static swsModule *produce(const std::string &typeName)
    {
        auto it = instanciators()->find(typeName);
        if  (it == instanciators()->end())
            throw sws::unknown_type(typeName);
        return it->second();
    }

protected:
    static std::unordered_map<std::string, swsModule *(*)()> *instanciators() {
        static std::unordered_map<std::string, swsModule *(*)()> instantiators;
        return &instantiators;
    }

    template<typename T>
    static swsModule* createInstance()
    {
        return new T();
    }
};

template<typename T>
class swsModuleRegistration
{
public:
    swsModuleRegistration()
    {
        printf("Registering type %s\n", T::getType().c_str());
        swsModuleFactory::registerModuleType<T>(T::getType());
        type = T::getType();
    }
    std::string type;
};

#endif // SWSMODULEFACTORY_H
