#ifndef STARSHIP_H
#define STARSHIP_H

#include <cassert>
#include <vector>
#include <iostream>

class Property {
protected:
    using Type = unsigned int;
    Type unit;
public:
    Property(Type unit) : unit(unit) {}

    Type getUnit() {
        return unit;
    }

    void operator+=(Property &&a) { unit += a.unit; }

    void operator+=(Property &a) { unit += a.unit; }

    void operator-=(Property &a) { unit -= a.unit; }

    void operator%=(Property &a) { unit %= a.unit; }

    friend bool operator<(Property &a, Property &b);

    friend bool operator==(Property &&a, Property &&b);

    friend bool operator<=(Property &a, Property &b);

    friend bool operator>=(Property &a, Property &&b);

    friend bool operator>(Property &a, Property &b);

    friend Type operator%(Property &a, Property &&b);

    friend Type operator%(Property &a, Property &b);

    friend std::ostream &operator<<(std::ostream &, const Property &);
};

class ShieldPoints : public Property {
public:
    ShieldPoints(Type unit) : Property(unit) {}
};

class Speed : public Property {
public:
    Speed(Type unit) noexcept : Property(unit) {}
};

class AttackPower : public Property {
public:
    AttackPower(Type unit) : Property(unit) {}
};

class Time : public Property {
public:
    Time(Type unit) : Property(unit) {}
};

class Timer {
public:
    virtual void moveTime(Time time) = 0;

    virtual bool isAttackTime() = 0;

    virtual Time getTime() = 0;
};

class ExemplaryTimer : public Timer {
private:
    Time actualTime;
    Time maxTime;
public:
    ExemplaryTimer(Time t0, Time t1) : actualTime(t0), maxTime(t1) {
        assert(actualTime >= Time(0));
        assert(actualTime <= maxTime);
        maxTime += 1;
    }

    void moveTime(Time time) override {
        actualTime += time;
        actualTime = actualTime % (maxTime);
    }

    bool isAttackTime() override {
        return ((actualTime % Time(2) == 0) || (actualTime % Time(3) == 0)) && (actualTime % Time(5));
    }

    Time getTime() override {
        return actualTime;
    }
};

class Ship {
public:
    virtual ShieldPoints getShield() = 0;

    virtual void takeDamage(AttackPower damage) = 0;

    virtual bool isDestroyed() = 0;

    virtual size_t getUnitsNumber() = 0;
};

class Attackable : public virtual Ship {
public:
    virtual AttackPower getAttackPower() = 0;
};

class BasicShip : public virtual Ship {
protected:
    ShieldPoints shieldPoints;
public:
    explicit BasicShip(ShieldPoints shieldPoints) : shieldPoints(shieldPoints) {}

    ShieldPoints getShield() override {
        return shieldPoints;
    }

    bool isDestroyed() override {
        return getShield() == ShieldPoints(0);
    }

    void takeDamage(AttackPower damage) override {
        if (shieldPoints <= damage) {
            shieldPoints = 0;
        } else {
            shieldPoints -= damage;
        }
    }

    size_t getUnitsNumber() override {
        if (isDestroyed())
            return 0;
        else
            return 1;
    }
};

class MobileShip : public virtual BasicShip {
protected:
    Speed speed;
public:
    MobileShip(ShieldPoints shieldPoints, Speed speed, Speed minSpeed,
               Speed maxSpeed) : BasicShip(shieldPoints), speed(speed) {
        assert(minSpeed <= speed && speed <= maxSpeed);
    }

    Speed getSpeed() {
        return speed;
    }
};


class Warship : public virtual BasicShip, public virtual Attackable {
protected:
    AttackPower attackPower;
public:
    Warship(ShieldPoints shieldPoints, AttackPower attackPower) : BasicShip(shieldPoints), attackPower(attackPower) {}

    AttackPower getAttackPower() override {
        if (!isDestroyed())
            return attackPower;
        else
            return 0;
    }
};

#endif //STARSHIP_H
