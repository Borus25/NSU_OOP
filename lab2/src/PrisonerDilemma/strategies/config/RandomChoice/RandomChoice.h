#pragma once

#include "../../basic_strategy/Strategy.h"
#include "../../basic_strategy/History/History.h"

class RandomChoice : public Strategy {
public:
    RandomChoice();

    Choice makeChoice(
        const History &opponent1History,
        const History &opponent2History
    ) override;
};
