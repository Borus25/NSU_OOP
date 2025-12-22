#pragma once

#include "../../basic_strategy/Strategy.h"
#include "../../basic_strategy/History/History.h"

class GoByMajority : public Strategy {
public:
    GoByMajority();
    
    Choice makeChoice(
        const History& opponent1History,
        const History& opponent2History
    ) override;
};
