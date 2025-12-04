#pragma once

#include <memory>
#include <string>
#include "../strategies/basic_strategy/Strategy.h"

class Player {
public:
    // Конструктор, принимает стратегию
    explicit Player(std::unique_ptr<Strategy> strategy);

    // Деструктор
    ~Player() = default;

    // Запрещаем копирование
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    // Разрешаем перемещение
    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    Choice makeChoice(const History& opponent1History,
                     const History& opponent2History);

    void updateHistory(Choice myChoice);

    void addScore(int points);

    [[nodiscard]] int getTotalScore() const;

    [[nodiscard]] const History& getHistory() const;

    [[nodiscard]] std::string getStrategyName() const;

    void reset();

private:
    std::unique_ptr<Strategy> strategy_;
    History history_;
    int totalScore_;
};
