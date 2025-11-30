#include "RandomChoice.h"

#include <ctime>
#include <random>

RandomChoice::RandomChoice() : Strategy("RandomChoice") {}

Choice RandomChoice::makeChoice(const History &myHistory, const History &opponent1History, const History &opponent2History) {
    static std::mt19937 generator(std::time(nullptr));
    static std::uniform_int_distribution<int> distribution(0, 1);

    // Генерируем случайное число: 0 или 1
    int random_value = distribution(generator);

    // Возвращаем COOPERATE или DEFECT
    return (random_value == 0) ? COOPERATE : DEFECT;
}
