#include "Dot.h"

Dot::Dot(int x, int y, int value) : Entity(x, y), collected(false), value(value) {
}

char Dot::getSymbol() const {
    return '.';
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
