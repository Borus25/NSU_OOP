#pragma once

#include <vector>

enum Choice {
    COOPERATE,
    DEFECT
};

class History {
public:
    History() = default;

    void addChoiceToHistory(Choice choice);

    [[nodiscard]] Choice getLastChoice() const;

    [[nodiscard]] Choice getIChoice(int i) const;

    [[nodiscard]] std::vector<Choice> getChoices() const;

    [[nodiscard]] bool isEmpty() const;
private:
    std::vector<Choice> choices;
};