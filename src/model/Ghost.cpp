#include "Ghost.h"

Ghost::Ghost(int x, int y, int id) : Entity(x, y), id(id) {
}

char Ghost::getSymbol() const {
    return '1' + id;
}

int Ghost::getId() const {
    return id;
}
