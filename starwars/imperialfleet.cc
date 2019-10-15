#include "imperialfleet.h"

std::shared_ptr<ImperialStarship> createDeathStar(ShieldPoints shieldPoints, AttackPower attackPower) {
    return std::make_shared<DeathStar>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialStarship> createImperialDestroyer(ShieldPoints shieldPoints, AttackPower attackPower) {
    return std::make_shared<ImperialDestroyer>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialStarship> createTIEFighter(ShieldPoints shieldPoints, AttackPower attackPower) {
    return std::make_shared<TIEFighter>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialStarship> createSquadron(std::vector<std::shared_ptr<ImperialStarship>> squadron) {
    return std::make_shared<Squadron>(squadron);
}

std::shared_ptr<ImperialStarship> createSquadron(std::initializer_list<std::shared_ptr<ImperialStarship>> list) {
    std::vector<std::shared_ptr<ImperialStarship>> squadron(list);
    return std::make_shared<Squadron>(squadron);
}