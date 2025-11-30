#include "SoftTitForTat.h"

SoftTitForTat::SoftTitForTat() : Strategy("SoftTitForTat") {}

Choice SoftTitForTat::makeChoice(
    const History& myHistory,
    const History& opponent1History,
    const History& opponent2History
) {
    // В первый раунд сотрудничаем
    if (opponent1History.isEmpty() && opponent2History.isEmpty()) {
        return COOPERATE;
    }
    
    // Предаём только если ОБА оппонента предали в прошлом раунде
    bool opponent1_defected = !opponent1History.isEmpty() && 
                              opponent1History.getLastChoice() == DEFECT;
    bool opponent2_defected = !opponent2History.isEmpty() && 
                              opponent2History.getLastChoice() == DEFECT;
    
    if (opponent1_defected && opponent2_defected) {
        return DEFECT;
    }
    
    // Иначе сотрудничаем
    return COOPERATE;
}
