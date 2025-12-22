#include "ToughTitForTat.h"

ToughTitForTat::ToughTitForTat() : Strategy("ToughTitForTat") {}

Choice ToughTitForTat::makeChoice(
    const History& opponent1History,
    const History& opponent2History
) {
    // В первый раунд сотрудничаем
    if (opponent1History.isEmpty() && opponent2History.isEmpty()) {
        return COOPERATE;
    }
    
    // Предаём, если хотя бы один из оппонентов предал в прошлом раунде
    if (!opponent1History.isEmpty() && opponent1History.getLastChoice() == DEFECT) {
        return DEFECT;
    }
    
    if (!opponent2History.isEmpty() && opponent2History.getLastChoice() == DEFECT) {
        return DEFECT;
    }
    
    // Иначе сотрудничаем
    return COOPERATE;
}
