#include "rebelfleet.h"

Speed Explorer::minSpeed = 299796;
Speed Explorer::maxSpeed = 2997960;


Speed XWing::minSpeed = 299796;
Speed XWing::maxSpeed = 2997960;

Speed StarCruiser::minSpeed = 99999;
Speed StarCruiser::maxSpeed = 299795;


std::shared_ptr<Explorer> createExplorer(ShieldPoints shield, Speed speed) {
    return std::make_shared<Explorer>(shield, speed);
}

std::shared_ptr<XWing> createXWing(ShieldPoints shield, Speed speed, AttackPower attackPower) {
    return std::make_shared<XWing>(shield, speed, attackPower);
}

std::shared_ptr<StarCruiser> createStarCruiser(ShieldPoints shield, Speed speed, AttackPower attackPower) {
    return std::make_shared<StarCruiser>(shield, speed, attackPower);
}


