#pragma once

#include <memory>
#include <vector>
#include <string>
#include "../Player/Player.h"
#include "../GameMatrix/GameMatrix.h"
#include "../strategies/basic_strategy/History/History.h"

class Game {
public:
    // Конструктор, принимает трёх игроков и матрицу
    Game(Player& player1, Player& player2, Player& player3, const std::string& mode="detailed");

    // Деструктор
    ~Game() = default;
    
    // Запрещаем копирование и перемещение
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
    
    // Проводит один раунд игры
    void playRound();
    
    // Проводит игру на заданное количество раундов
    void playGame(int numRounds);
    
    // Проводит детальный режим (с ожиданием ввода)
    void playDetailedMode();
    
    // Выводит результаты раунда
    void displayRoundResult(int roundNumber, 
                           Choice c1, Choice c2, Choice c3,
                           int score1, int score2, int score3);

    // Выводит итоговые результаты игры
    void displayGameResult() const;
    
    // Возвращает счета всех игроков
    [[nodiscard]] std::vector<int> getScores() const;
    
    // Возвращает победителя (индекс 0, 1 или 2)
    [[nodiscard]] int getWinner() const;
    
    // Сбрасывает игру (очищает истории и счета)
    void reset();

private:
    std::vector<Player*> players_;
    GameMatrix gameMatrix_;
    std::string mode_;
    int currentRound_;
};
