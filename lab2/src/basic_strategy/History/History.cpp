#include "History.h"

std::vector<Choice> History::getChoices() const {
    return choices;
}

void History::addChoiceToHistory(Choice choice) {
    choices.push_back(choice);
}

Choice History::getLastChoice() const {
    return choices.back();
}

Choice History::getIChoice(int i) const {
    return choices.at(i);
}

bool History::isEmpty() const {
    return choices.empty();
}



