#ifndef SWSFACTORY_H
#define SWSFACTORY_H

#include <string>
#include <unordered_map>

#include "swsexception.h"

// Inspired from http://www.nirfriedman.com/2018/04/29/unforgettable-factory/

template <class Base>
class swsFactory
{
public:
    static Base *produce(const std::string &typeName)
    {
        auto it = instanciators().find(typeName);
        if  (it == instanciators().end())
            throw sws::unknown_type(typeName);
        return it->second();
    }

    template<typename T>
    struct Registrar: Base {
        friend T;

        static bool registerT() {
            const auto typeName = T::getType();
            auto it = instanciators().find(typeName);
            if (it != instanciators().end())
                throw sws::duplicate_name();

            swsFactory::instanciators()[typeName] = []() -> Base* { return new T(); };
            return true;
        };
        static bool registered;

    private:
        Registrar() { (void)registered; }
    };

    friend Base;

private:
    using FuncType = Base *(*)();
    swsFactory() = default;

    static std::unordered_map<std::string, FuncType> &instanciators() {
        static std::unordered_map<std::string, FuncType> instantiators;
        return instantiators;
    }
};

template <class Base>
template <class T>
bool swsFactory<Base>::Registrar<T>::registered =
    swsFactory<Base>::Registrar<T>::registerT();

#endif // SWSFACTORY_H
