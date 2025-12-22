#pragma once

#include "../../basic_strategy/Strategy.h"
#include "../../basic_strategy/History/History.h"

class ToughTitForTat : public Strategy {
public:
    ToughTitForTat();
    
    Choice makeChoice(
        const History& opponent1History,
        const History& opponent2History
    ) override;
};