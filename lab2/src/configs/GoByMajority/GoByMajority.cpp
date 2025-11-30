#include "GoByMajority.h"

GoByMajority::GoByMajority() : Strategy("GoByMajority") {}

Choice GoByMajority::makeChoice(
    const History& myHistory,
    const History& opponent1History,
    const History& opponent2History
) {
    // В первый раунд сотрудничаем
    if (opponent1History.isEmpty() && opponent2History.isEmpty()) {
        return COOPERATE;
    }
    
    // Подсчитываем количество сотрудничеств и предательств оппонентов
    int cooperate_count = 0;
    int defect_count = 0;
    
    // Подсчёт для первого оппонента
    if (!opponent1History.isEmpty()) {
        const std::vector<Choice>& choices1 = opponent1History.getChoices();
        for (Choice c : choices1) {
            if (c == COOPERATE) {
                cooperate_count++;
            } else {
                defect_count++;
            }
        }
    }
    
    // Подсчёт для второго оппонента
    if (!opponent2History.isEmpty()) {
        const std::vector<Choice>& choices2 = opponent2History.getChoices();
        for (Choice c : choices2) {
            if (c == COOPERATE) {
                cooperate_count++;
            } else {
                defect_count++;
            }
        }
    }
    
    // Выбираем большинство
    if (defect_count > cooperate_count) {
        return DEFECT;
    }
    return COOPERATE;
}
