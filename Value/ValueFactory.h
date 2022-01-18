//
// Created by radua on 1/18/2022.
//

#ifndef OOP_PROJ_VALUEFACTORY_H
#define OOP_PROJ_VALUEFACTORY_H

#include <memory>
#include <functional>
#include <map>
#include "value.h"
#include "../Ptr/Ptr.h"
#include <iostream>

/*
 * Implementare inspirata de articolul despre object factories gasit la adresa
 * https://www.codeproject.com/Articles/567242/AplusC-2b-2bplusObjectplusFactory
 */


class ValueFactory {
    std::map<std::string, std::function<Base*(void)>> functionRegistry;
public:
    Ptr<Base> createValue(const std::string& type);
    void registerFactoryFunction(const std::string&, std::function<Base*(void)>);
    static ValueFactory *Instance();
};

template<class T>
class Registrar {
public:
    explicit Registrar(const std::string& type) {
        ValueFactory::Instance()->
            registerFactoryFunction(type,
                                    []() -> Base * { return new T(); });
    }
};

#endif //OOP_PROJ_VALUEFACTORY_H
