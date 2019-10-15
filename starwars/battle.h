#ifndef BATTLE_H
#define BATTLE_H

#include <vector>
#include <iostream>
#include <utility>
#include "rebelfleet.h"
#include "imperialfleet.h"
#include "helper.h"

class SpaceBattle {
public:
    class Builder;

    SpaceBattle(
            Time t0,
            Time t1,
            std::vector<std::shared_ptr<RebelStarship>> rebelStarships,
            std::vector<std::shared_ptr<ImperialStarship>> imperialStarships) :
            rebelStarships(std::move(rebelStarships)),
            imperialStarships(std::move(imperialStarships)) {
        timer = new ExemplaryTimer(t0, t1);
    }

    size_t countImperialFleet() {
        size_t imperialStarshipsNumber = 0;
        for (auto unit : imperialStarships) {
            imperialStarshipsNumber += unit->getUnitsNumber();
        }
        return imperialStarshipsNumber;
    }

    size_t countRebelFleet() {
        size_t rebelStarshipsNumber = 0;
        for (auto unit : rebelStarships) {
            rebelStarshipsNumber += unit->getUnitsNumber();
        }
        return rebelStarshipsNumber;
    }

    void tick(Time timeStep) {
        if (countImperialFleet() == 0 && countRebelFleet() == 0) {
            std::cout << "DRAW" << std::endl;
        } else if (countImperialFleet() == 0) {
            std::cout << "REBELLION WON" << std::endl;
        } else if (countRebelFleet() == 0) {
            std::cout << "IMPERIUM WON" << std::endl;
        }
        if (timer->isAttackTime()) {
            for (auto imperialStarship : imperialStarships) {
                for (auto rebelStarship : rebelStarships) {
                    if (!imperialStarship->isDestroyed() && !rebelStarship->isDestroyed()) {
                        rebelStarship->receiveAttack(imperialStarship.get());
                    }
                }
            }
        }
        timer->moveTime(timeStep);
    }

private:
    Timer *timer;
    std::vector<std::shared_ptr<RebelStarship>> rebelStarships;
    std::vector<std::shared_ptr<ImperialStarship>> imperialStarships;
};

class SpaceBattle::Builder {
public:
    Builder &startTime(Time t) {
        t0 = t;
        return *this;
    }

    Builder &maxTime(Time t) {
        t1 = t;
        return *this;
    }

    Builder &ship(std::shared_ptr<RebelStarship> ship) {
        rebelStarships.push_back(ship);
        return *this;
    }

    Builder &ship(std::shared_ptr<ImperialStarship> ship) {
        imperialStarships.push_back(ship);
        return *this;
    }

    SpaceBattle build() const {
        return SpaceBattle(t0, t1, rebelStarships, imperialStarships);
    }

private:
    Time t0 = 0;
    Time t1 = 1;
    std::vector<std::shared_ptr<RebelStarship>> rebelStarships;
    std::vector<std::shared_ptr<ImperialStarship>> imperialStarships;
};

#endif //BATTLE_H