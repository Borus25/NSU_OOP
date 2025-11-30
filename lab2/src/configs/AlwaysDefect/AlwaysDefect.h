#pragma once

#include "../../basic_strategy/Strategy.h"
#include "../../basic_strategy/History/History.h"

class AlwaysDefect : public Strategy {
public:
    AlwaysDefect();
    
    Choice makeChoice(
        const History& myHistory,
        const History& opponent1History,
        const History& opponent2History
    ) override;
};
