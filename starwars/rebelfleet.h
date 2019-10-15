#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <memory>
#include "helper.h"
#include "imperialfleet.h"

class RebelStarship : public MobileShip {
public:
    RebelStarship(ShieldPoints shield, Speed speed, Speed minSpeed,
                  Speed maxSpeed) :
            MobileShip(shield, speed, minSpeed, maxSpeed),
            BasicShip(shield) {
    }

    virtual void receiveAttack(ImperialStarship *attacker) {
        takeDamage(attacker->getAttackPower());
    }
};

class RebelWarship : public RebelStarship, public Warship {
public:
    RebelWarship(ShieldPoints shield, Speed speed, AttackPower attack, Speed minSpeed,
                 Speed maxSpeed) :
            RebelStarship(shield, speed, minSpeed, maxSpeed),
            Warship(shield, attack),
            BasicShip(shield) {
    }

    void receiveAttack(ImperialStarship *attacker) override {
        AttackPower attackPower = getAttackPower();
        takeDamage(attacker->getAttackPower());
        attacker->takeDamage(attackPower);
    }
};

class Explorer : public RebelStarship {
    static Speed minSpeed;
    static Speed maxSpeed;
public:
    Explorer(ShieldPoints shield, Speed speed) :
            RebelStarship(shield, speed, minSpeed, maxSpeed),
            BasicShip(shield) {
    }
};

class XWing : public RebelWarship {
protected:
    static Speed minSpeed;
    static Speed maxSpeed;
public:
    XWing(ShieldPoints shield, Speed speed, AttackPower attack) :
            RebelWarship(shield, speed, attack, minSpeed, maxSpeed),
            BasicShip(shield) {
    }
};


class StarCruiser : public RebelWarship {
    static Speed minSpeed;
    static Speed maxSpeed;
public:
    StarCruiser(ShieldPoints shield, Speed speed, AttackPower attack) :
            RebelWarship(shield, speed, attack, minSpeed, maxSpeed),
            BasicShip(shield) {
    }
};

std::shared_ptr<Explorer> createExplorer(ShieldPoints shield, Speed speed);

std::shared_ptr<XWing> createXWing(ShieldPoints shield, Speed speed, AttackPower attackPower);

std::shared_ptr<StarCruiser> createStarCruiser(ShieldPoints shield, Speed speed, AttackPower attackPower);


#endif //REBELFLEET_H
