#include "History.h"
#include <stdexcept>

std::vector<Choice> History::getChoices() const {
    return choices;
}

void History::addChoiceToHistory(Choice choice) {
    choices.push_back(choice);
}

Choice History::getLastChoice() const {
    // Случай 11: getLastChoice() на пустой истории
    if (choices.empty()) {
        throw std::out_of_range("Cannot get last choice from empty history");
    }
    return choices.back();
}

Choice History::getIChoice(int i) const {
    // Случай 12: getIChoice() с неверным индексом
    // Метод at() автоматически выбросит std::out_of_range
    return choices.at(i);
}

bool History::isEmpty() const {
    return choices.empty();
}



