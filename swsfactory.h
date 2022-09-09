#ifndef SWSFACTORY_H
#define SWSFACTORY_H

#include <string>
#include <unordered_map>

#include "swsexception.h"

// Inspired from http://www.nirfriedman.com/2018/04/29/unforgettable-factory/

class swsSchema;

template <class Base>
class swsFactory
{
public:
    static Base *produce(swsSchema *schema, const std::string &typeName)
    {
        auto it = instanciators().find(typeName);
        if  (it == instanciators().end())
            throw sws::unknown_type(typeName);
        return it->second(schema);
    }

    template<typename T>
    struct Registrar: Base {
        friend T;

        static bool registerT() {
            const auto typeName = T::getType();
            auto it = instanciators().find(typeName);
            if (it != instanciators().end())
                throw sws::duplicate_name();

            swsFactory::instanciators()[typeName] = [](swsSchema *schema) -> Base* { return new T(schema); };
            return true;
        };
        static bool registered;

    private:
        Registrar(swsSchema *schema) : swsModule(schema) { (void)registered; }
    };

    friend Base;

private:
    using FuncType = Base *(*)(swsSchema *);
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
