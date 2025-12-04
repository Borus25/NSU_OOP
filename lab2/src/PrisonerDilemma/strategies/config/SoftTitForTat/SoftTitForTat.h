#pragma once

#include "../../basic_strategy/Strategy.h"
#include "../../basic_strategy/History/History.h"

class SoftTitForTat : public Strategy {
public:
    SoftTitForTat();
    
    Choice makeChoice(
        const History& opponent1History,
        const History& opponent2History
    ) override;
};
