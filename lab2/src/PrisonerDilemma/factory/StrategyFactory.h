#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>

#include "../strategies/basic_strategy/Strategy.h"

template<typename ID_T = std::string,
         typename Product_T = Strategy>
class Factory {
public:
    using Creator_T = std::function<std::unique_ptr<Product_T>()>;

    static Factory* getInstance() {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Product_T> create_by_name(const ID_T& name) {
        auto it = creators.find(name);
        if (it == creators.end())
            throw std::out_of_range("This strategy '" + name + "' don't exist");
        return it->second();
    }

    bool register_strategy(const ID_T& name, Creator_T creator) {
        if (creators.find(name) == creators.end()) {
            creators[name] = creator;
            return true;
        }
        return false;
    }

private:
    Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    std::map<ID_T, Creator_T> creators;
};

using StrategyFactory = Factory<std::string, Strategy>;
