#include "Game.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

Game::Game(Player& player1, Player& player2, Player& player3, const std::string& mode)
    : players_{&player1, &player2, &player3},
      gameMatrix_(player1.getStrategyName(),
                 player2.getStrategyName(),
                 player3.getStrategyName()),
      mode_(mode),
      currentRound_(0) {
    // Случай 26: Game с одинаковыми Player объектами
    if (&player1 == &player2 || &player2 == &player3 || &player1 == &player3) {
        throw std::invalid_argument("All players must be different objects");
    }
}

void Game::playRound() {
    currentRound_++;
    
    // Каждый игрок делает выбор
    Choice choice1 = players_[0]->makeChoice(players_[1]->getHistory(), players_[2]->getHistory());
    Choice choice2 = players_[1]->makeChoice(players_[0]->getHistory(), players_[2]->getHistory());
    Choice choice3 = players_[2]->makeChoice(players_[0]->getHistory(), players_[1]->getHistory());

    // Вычисляем очки за раунд
    int cooperators = (choice1 == COOPERATE) + (choice2 == COOPERATE) + (choice3 == COOPERATE);

    auto getScore = [cooperators](Choice myChoice) -> int {
        if (cooperators == 3) return 7;                    // CCC
        if (cooperators == 2) return (myChoice == COOPERATE) ? 3 : 9;  // 2C1D
        if (cooperators == 1) return (myChoice == COOPERATE) ? 0 : 5;  // 1C2D
        return 1;                                          // DDD
    };
    
    int score1 = getScore(choice1);
    int score2 = getScore(choice2);
    int score3 = getScore(choice3);

    // Добавляем очки игрокам
    players_[0]->addScore(score1);
    players_[1]->addScore(score2);
    players_[2]->addScore(score3);

    // Обновляем историю
    players_[0]->updateHistory(choice1);
    players_[1]->updateHistory(choice2);
    players_[2]->updateHistory(choice3);

    // Сохраняем результат в матрице
    gameMatrix_.calculateScores(choice1, choice2, choice3);

    // Выводим результаты раунда
    displayRoundResult(currentRound_, choice1, choice2, choice3, score1, score2, score3);
}

void Game::playGame(int numRounds) {
    std::cout << "\n=== Starting Game ===" << std::endl;
    std::cout << "Players: "
              << players_[0]->getStrategyName() << ", "
              << players_[1]->getStrategyName() << ", "
              << players_[2]->getStrategyName() << std::endl;
    std::cout << "Rounds: " << numRounds << std::endl;
    std::cout << "========================\n" << std::endl;
    
    for (int i = 0; i < numRounds; ++i) {
        playRound();
    }
    
    displayGameResult();
}

void Game::playDetailedMode() {
    std::cout << "\n=== Detailed Mode ===" << std::endl;
    std::cout << "Players: "
              << players_[0]->getStrategyName() << ", "
              << players_[1]->getStrategyName() << ", "
              << players_[2]->getStrategyName() << std::endl;
    std::cout << "Press Enter to play next round, or type 'quit' to exit.\n" << std::endl;
    
    std::string input;
    while (true) {
        std::cout << "Press Enter for round " << (currentRound_ + 1) << "...";

        // Случай 24: обработка EOF
        if (!std::getline(std::cin, input)) {
            std::cout << "\nEnd of input (EOF). Game terminated." << std::endl;
            break;
        }

        if (input == "quit" || input == "q") {
            std::cout << "\nGame terminated by user." << std::endl;
            break;
        }
        
        playRound();
        
        // Показываем текущие счета
        std::cout << "Current scores: "
                  << players_[0]->getStrategyName() << "=" << players_[0]->getTotalScore() << ", "
                  << players_[1]->getStrategyName() << "=" << players_[1]->getTotalScore() << ", "
                  << players_[2]->getStrategyName() << "=" << players_[2]->getTotalScore()
                  << "\n" << std::endl;
    }
    
    displayGameResult();
}

void Game::displayRoundResult(int roundNumber, 
                              Choice c1, Choice c2, Choice c3,
                              int score1, int score2, int score3) {
    auto choiceToString = [](Choice c) -> std::string {
        return (c == COOPERATE) ? "C" : "D";
    };
    
    std::cout << "Round " << roundNumber << ": "
              << players_[0]->getStrategyName() << "=" << choiceToString(c1) << "(" << score1 << "), "
              << players_[1]->getStrategyName() << "=" << choiceToString(c2) << "(" << score2 << "), "
              << players_[2]->getStrategyName() << "=" << choiceToString(c3) << "(" << score3 << ")"
              << std::endl;
}

void Game::displayGameResult() const {
    std::cout << "\n=== Game Results ===" << std::endl;
    if (mode_ == "detailed" || mode_ == "tournament") {
        std::cout << "\n=== Scores Matrix ===" << std::endl;
        gameMatrix_.printMatrix();
    } else {
        std::cout << players_[0]->getStrategyName() << ": " << players_[0]->getTotalScore() << " points" << std::endl;
        std::cout << players_[1]->getStrategyName() << ": " << players_[1]->getTotalScore() << " points" << std::endl;
        std::cout << players_[2]->getStrategyName() << ": " << players_[2]->getTotalScore() << " points" << std::endl;
    }

    int winner = getWinner();
    std::cout << "\nWinner: " << players_[winner]->getStrategyName() << "!" << std::endl;
}

std::vector<long long> Game::getScores() const {
    return {
        players_[0]->getTotalScore(),
        players_[1]->getTotalScore(),
        players_[2]->getTotalScore()
    };
}

int Game::getWinner() const {
    // Случай 25: getWinner() при нескольких победителях (ничья)
    // Возвращает индекс ПЕРВОГО игрока с максимальным счетом
    // Например, если все имеют одинаковый счет, вернет 0
    std::vector<long long> scores = getScores();
    auto maxIt = std::max_element(scores.begin(), scores.end());
    return std::distance(scores.begin(), maxIt);
}

void Game::reset() {
    players_[0]->reset();
    players_[1]->reset();
    players_[2]->reset();
    currentRound_ = 0;
    gameMatrix_ = GameMatrix(players_[0]->getStrategyName(),
                            players_[1]->getStrategyName(),
                            players_[2]->getStrategyName());
}
