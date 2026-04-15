#include "Ghost.h"
#include "GhostStrategy.h"

Ghost::Ghost(int x, int y, int id, GhostStrategy* strategy)
    : Entity(x, y), id(id), strategy(strategy) {
}

char Ghost::getSymbol() const {
    return '1' + id;
}

int Ghost::getId() const {
    return id;
}

Direction Ghost::chooseDirection(const Game& game) const {
    if (strategy) return strategy->chooseDirection(*this, game);
    return Direction::None;
}
