#pragma once

#include "History/History.h"
#include <string>

class Strategy {
public:
    explicit Strategy(const std::string& strategyName)
        : name(strategyName) {}
    virtual ~Strategy() = default;

    virtual Choice makeChoice(
        const History& opponent1History,
        const History& opponent2History
    ) = 0;

    [[nodiscard]] virtual std::string getName() const {
        return name;
    }

    // virtual void reset();

    // virtual void loadConfig(const std::string& configPath);
private:
    std::string name;
};