#include "Player.h"

class Strategy;

Player::Player(std::unique_ptr<Strategy> strategy)
    : strategy_(std::move(strategy)),
      totalScore_(0) {}

Choice Player::makeChoice(const History& opponent1History,
                         const History& opponent2History) {
    return strategy_->makeChoice(opponent1History, opponent2History);
}

void Player::updateHistory(Choice myChoice) {
    history_.addChoiceToHistory(myChoice);
}

void Player::addScore(int points) {
    totalScore_ += points;
}

int Player::getTotalScore() const {
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
