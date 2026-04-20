#include "Dot.h"

Dot::Dot(int x, int y, int value, bool powerPellet)
    : Entity(x, y), collected(false), value(value), powerPellet(powerPellet) {
}

char Dot::getSymbol() const {
    return powerPellet ? 'O' : '.';
}

bool Dot::isPowerPellet() const {
    return powerPellet;
}

bool Dot::isCollected() const {
    return collected;
}

int Dot::getValue() const {
    return value;
}

void Dot::collect() {
    collected = true;
}

void Dot::setCollected(bool c) {
    collected = c;
}
