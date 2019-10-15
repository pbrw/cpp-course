#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include "helper.h"
#include <vector>
#include <memory>

class ImperialStarship : public virtual Attackable {
public:
    ImperialStarship() = default;
};

class ImperialUnit : public Warship {
public:
    ImperialUnit(ShieldPoints shieldPoints, AttackPower attackPower) :
            Warship(shieldPoints, attackPower),
            BasicShip(shieldPoints) {
    }
};

class DeathStar : public ImperialStarship, public ImperialUnit {
public:
    DeathStar(ShieldPoints shieldPoints, AttackPower attackPower) :
            ImperialUnit(shieldPoints, attackPower),
            BasicShip(shieldPoints) {
    }
};

class ImperialDestroyer : public ImperialStarship, public ImperialUnit {
public:
    ImperialDestroyer(ShieldPoints shieldPoints, AttackPower attackPower) :
            ImperialUnit(shieldPoints, attackPower),
            BasicShip(shieldPoints) {
    }
};

class TIEFighter : public ImperialStarship, public ImperialUnit {
public:
    TIEFighter(ShieldPoints shieldPoints, AttackPower attackPower) :
            ImperialUnit(shieldPoints, attackPower),
            BasicShip(shieldPoints) {
    }
};

class Squadron : public ImperialStarship {
private:
    std::vector<std::shared_ptr<ImperialStarship>> squadron;
public:
    Squadron(std::vector<std::shared_ptr<ImperialStarship>> &squadron) : squadron(squadron) {
    }

    ShieldPoints getShield() override {
        ShieldPoints shieldPoints = 0;
        for (auto &starship : squadron) {
            shieldPoints += starship->getShield();
        }
        return shieldPoints;
    }

    bool isDestroyed() override {
        return getShield() == ShieldPoints(0);
    }

    void takeDamage(AttackPower damage) override {
        for (auto &starship : squadron) {
            starship->takeDamage(damage);
        }
    }

    AttackPower getAttackPower() override {
        AttackPower attackPower = 0;
        for (auto &starship : squadron) {
            attackPower += starship->getAttackPower();
        }
        return attackPower;
    }

    size_t getUnitsNumber() override {
        size_t unitsNumber = 0;
        for (auto &starship : squadron) {
            unitsNumber += starship->getUnitsNumber();
        }
        return unitsNumber;
    }
};

std::shared_ptr<ImperialStarship> createDeathStar(ShieldPoints shieldPoints, AttackPower attackPower);

std::shared_ptr<ImperialStarship> createImperialDestroyer(ShieldPoints shieldPoints, AttackPower attackPower);

std::shared_ptr<ImperialStarship> createTIEFighter(ShieldPoints shieldPoints, AttackPower attackPower);

std::shared_ptr<ImperialStarship> createSquadron(std::vector<std::shared_ptr<ImperialStarship>> squadron);

std::shared_ptr<ImperialStarship> createSquadron(std::initializer_list<std::shared_ptr<ImperialStarship>> list);

#endif //IMPERIALFLEET_H
