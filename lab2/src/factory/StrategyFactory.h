#pragma once

#include <map>
#include <memory>
#include <string>

#include "../basic_strategy/Strategy.h"

template<typename ID_T = std::string,
         typename Product_T = Strategy,
         typename Creator_T = Product_T*(*)()>
class Factory {
public:
    static Factory* getInstance() {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Product_T> create_by_name(const ID_T& name) {
        auto it = creators.find(name);
        if (it == creators.end())
            return nullptr;
        return std::unique_ptr<Product_T>(it->second());
    }

    bool register_strategy(const ID_T& name, Creator_T creator) {
        if (creators.find(name) == creators.end()) {
            creators[name] = creator;
        }
        return true;
    }

private:
    Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    std::map<ID_T, Creator_T> creators;
};

using StrategyFactory = Factory<std::string, Strategy, Strategy*(*)()>;
