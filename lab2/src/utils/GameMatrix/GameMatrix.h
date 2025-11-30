#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "../../basic_strategy/History/History.h"

using Round = std::tuple<short,short,short>;

class GameMatrix {
public:
    GameMatrix(const std::string& strategyName1,
               const std::string& strategyName2,
               const std::string& strategyName3);

    // void loadFromFile(const std::string& filename);

    void calculateScores(Choice c1, Choice c2, Choice c3);

    void printMatrix() const;

private:
    std::vector<Round> matrixScores;
    std::vector<std::string> strategyNames;

    int sumScores1 = 0;
    int sumScores2 = 0;
    int sumScores3 = 0;
};