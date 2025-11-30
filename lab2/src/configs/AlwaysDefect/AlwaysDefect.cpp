#include "AlwaysDefect.h"

AlwaysDefect::AlwaysDefect() : Strategy("AlwaysDefect") {}

Choice AlwaysDefect::makeChoice(const History &myHistory, const History &opponent1History, const History &opponent2History) {
    return DEFECT;
}

