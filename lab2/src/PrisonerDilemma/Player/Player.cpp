#include "Player.h"
#include <stdexcept>

class Strategy;

Player::Player(std::unique_ptr<Strategy> strategy)
    : strategy_(std::move(strategy)),
      totalScore_(0) {
    // Случай 16: Player без стратегии (nullptr)
    if (!strategy_) {
        throw std::invalid_argument("Strategy cannot be null");
    }
}

Choice Player::makeChoice(const History& opponent1History,
                         const History& opponent2History) {
    return strategy_->makeChoice(opponent1History, opponent2History);
}

void Player::updateHistory(Choice myChoice) {
    history_.addChoiceToHistory(myChoice);
}

void Player::addScore(int points) {
    // Случай 14: отрицательные очки - добавляем проверку
    if (points < 0) {
        throw std::invalid_argument("Score points cannot be negative");
    }
    // Случай 15: целочисленное переполнение (используем long long в заголовке)
    totalScore_ += points;
}

long long Player::getTotalScore() const {
    return totalScore_;
}

const History& Player::getHistory() const {
    return history_;
}

std::string Player::getStrategyName() const {
    return strategy_->getName();
}

void Player::reset() {
    history_ = History();
    totalScore_ = 0;
}
