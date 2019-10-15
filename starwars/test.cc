//
// Created by piotr on 13.01.19.
//

#include "helper.h"
#include "rebelfleet.h"
#include "imperialfleet.h"
#include "battle.h"
#include <memory>
#include <iostream>
#include <cassert>

using namespace std;

void test1() {
    auto xwing = createXWing(500, 300000, 50);
    auto fighter1 = createTIEFighter(50, 9);
    auto fighter2 = createTIEFighter(50, 9);
    auto fighter3 = createTIEFighter(50, 9);
    auto fighter4 = createTIEFighter(50, 9);
    auto fighter5 = createTIEFighter(50, 9);
    auto squadron = createSquadron({fighter1, fighter2, fighter3, fighter4, fighter5});
    auto fighter6 = createTIEFighter(50, 9);
    auto squadron_squared = createSquadron({squadron, fighter6});

    auto battle = SpaceBattle::Builder()
            .ship(squadron_squared)
            .startTime(24)
            .maxTime(30)
            .ship(xwing)
            .build();

    assert(battle.countImperialFleet() == 6);
    assert(battle.countRebelFleet() == 1);

    battle.tick(1);

    assert(battle.countImperialFleet() == 0);
    assert(battle.countRebelFleet() == 1);
    assert(xwing->getShield() == 500 - 9 * 6);
}

void test2() {
    auto xwing = createXWing(200, 300000, 50);
    auto fighter1 = createTIEFighter(50, 9);
    auto fighter2 = createTIEFighter(50, 9);
    auto fighter3 = createTIEFighter(50, 9);
    auto fighter4 = createTIEFighter(50, 9);
    auto fighter5 = createTIEFighter(51, 9);
    auto squadron = createSquadron({fighter1, fighter2, fighter3, fighter4, fighter5});
    auto fighter6 = createTIEFighter(50, 9);
    auto squadron_squared = createSquadron({squadron, fighter6});

    auto battle = SpaceBattle::Builder()
            .ship(squadron_squared)
            .startTime(24)
            .maxTime(30)
            .ship(xwing)
            .build();

    assert(battle.countImperialFleet() == 6);
    assert(battle.countRebelFleet() == 1);

    battle.tick(1);

    assert(battle.countImperialFleet() == 1);
    assert(battle.countRebelFleet() == 1);
    assert(xwing->getShield() == 200 - 9 * 6);

    battle.tick(2);

    assert(battle.countImperialFleet() == 1);
    assert(battle.countRebelFleet() == 1);
    assert(xwing->getShield() == 200 - 9 * 6);

    battle.tick(7);

    assert(battle.countImperialFleet() == 0);
    assert(battle.countRebelFleet() == 1);
    assert(xwing->getShield() == 200 - 9 * 6 - 9);
}

void test3() {
    auto xwing = createXWing(200, 300000, 20);
    auto explorer = createExplorer(50, 330111);
    auto star_cruiser = createStarCruiser(250, 120979, 25);
    auto fighter = createTIEFighter(50, 10);
    auto fighter2 = createTIEFighter(55, 10);
    auto destroyer = createImperialDestroyer(60, 15);
    auto destroyer2 = createImperialDestroyer(65, 15);
    auto death_star = createDeathStar(100, 30);
    auto squadron = createSquadron({fighter2, destroyer2});

    auto battle = SpaceBattle::Builder()
            .ship(xwing)
            .ship(explorer)
            .ship(star_cruiser)
            .ship(fighter)
            .ship(destroyer)
            .ship(death_star)
            .ship(squadron)
            .startTime(2)
            .maxTime(4)
            .build();

    assert(battle.countImperialFleet() == 5);
    assert(battle.countRebelFleet() == 3);

    battle.tick(1);

    assert(battle.countImperialFleet() == 5);
    assert(battle.countRebelFleet() == 2);
    assert(xwing->getShield() == 120);
    assert(explorer->getShield() == 0);
    assert(star_cruiser->getShield() == 170);
    assert(fighter->getShield() == 5);
    assert(fighter2->getShield() == 10);
    assert(destroyer->getShield() == 15);
    assert(destroyer2->getShield() == 20);
    assert(death_star->getShield() == 55);
    assert(squadron->getShield() == 30);

    battle.tick(2);

    assert(battle.countImperialFleet() == 1);
    assert(battle.countRebelFleet() == 2);
    assert(xwing->getShield() == 40);
    assert(explorer->getShield() == 0);
    assert(star_cruiser->getShield() == 140);
    assert(fighter->getShield() == 0);
    assert(fighter2->getShield() == 0);
    assert(destroyer->getShield() == 0);
    assert(destroyer2->getShield() == 0);
    assert(death_star->getShield() == 10);
    assert(squadron->getShield() == 0);

    battle.tick(2);

    assert(battle.countImperialFleet() == 1);
    assert(battle.countRebelFleet() == 2);
    assert(xwing->getShield() == 40);
    assert(explorer->getShield() == 0);
    assert(star_cruiser->getShield() == 140);
    assert(fighter->getShield() == 0);
    assert(fighter2->getShield() == 0);
    assert(destroyer->getShield() == 0);
    assert(destroyer2->getShield() == 0);
    assert(death_star->getShield() == 10);
    assert(squadron->getShield() == 0);

    battle.tick(1);

    assert(battle.countImperialFleet() == 0);
    assert(battle.countRebelFleet() == 2);
    assert(xwing->getShield() == 10);
    assert(explorer->getShield() == 0);
    assert(star_cruiser->getShield() == 140);
    assert(fighter->getShield() == 0);
    assert(fighter2->getShield() == 0);
    assert(destroyer->getShield() == 0);
    assert(destroyer2->getShield() == 0);
    assert(death_star->getShield() == 0);
    assert(squadron->getShield() == 0);
}


int main() {
    test1();
    test2();
    test3();
}