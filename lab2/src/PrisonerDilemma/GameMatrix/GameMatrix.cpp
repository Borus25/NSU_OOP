#include "GameMatrix.h"

#include <iostream>

GameMatrix::GameMatrix(const std::string &strategyName1,
                       const std::string &strategyName2,
                       const std::string &strategyName3) : strategyNames{strategyName1, strategyName2, strategyName3} {}

void GameMatrix::printMatrix() const {
    std::cout << "Round\t";
    for (const std::string& strategyName : strategyNames) {
        std::cout << strategyName << '\t';
    }
    std::cout << std::endl;

    int roundNumber = 1;
    for (const Round& round : matrixScores) {
        std::cout << roundNumber << '\t'
                  << std::get<0>(round) << "\t\t"
                  << std::get<1>(round) << "\t\t"
                  << std::get<2>(round) << std::endl;
        roundNumber++;
    }

    std::cout << "=== Total Scores ===" << std::endl;
    std::cout << '\t' << sumScores1 << "\t\t";
    std::cout << sumScores2 << "\t\t";
    std::cout << sumScores3 << std::endl;
}

void GameMatrix::calculateScores(Choice c1, Choice c2, Choice c3) {
    int cooperators = (c1 == COOPERATE) + (c2 == COOPERATE) + (c3 == COOPERATE);

    // Формула для вычисления очков на основе количества сотрудничающих
    auto getScore = [cooperators](Choice myChoice) -> short {
        if (cooperators == 3) return 7;                    // CCC
        if (cooperators == 2) return (myChoice == COOPERATE) ? 3 : 9;  // 2C1D
        if (cooperators == 1) return (myChoice == COOPERATE) ? 0 : 5;  // 1C2D
        return 1;                                          // DDD
    };

    short score1 = getScore(c1);
    short score2 = getScore(c2);
    short score3 = getScore(c3);

    Round round = {score1, score2, score3};

    matrixScores.push_back(round);

    sumScores1 += score1;
    sumScores2 += score2;
    sumScores3 += score3;
}



