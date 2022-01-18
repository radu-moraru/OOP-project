//
// Created by radua on 1/18/2022.
//

#include "ValueFactory.h"

void ValueFactory::registerFactoryFunction(const std::string& type,
                                           std::function<Base *()> factoryFunction) {
    // maps function to type string
    functionRegistry[type] = factoryFunction;
}

Ptr<Base> ValueFactory::createValue(const std::string& type) {
    Base *ptr = nullptr;

    // iterator catre functia mapata la sirul de caractere type
    auto it = functionRegistry.find(type);

    if (it != functionRegistry.end())
        ptr = it->second();
    else
        throw std::runtime_error("Type name not mapped to an object factory function.");

    return Ptr<Base>(ptr);
}

ValueFactory *ValueFactory::Instance() {
    static ValueFactory factory;
    return &factory;
}