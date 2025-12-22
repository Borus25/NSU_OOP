#include "AlwaysCooperate.h"

AlwaysCooperate::AlwaysCooperate() : Strategy("AlwaysCooperate") {}

Choice AlwaysCooperate::makeChoice(const History &opponent1History, const History &opponent2History) {
    return COOPERATE;
}



