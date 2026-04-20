#include "Ghost.h"
#include "GhostStrategy.h"
#include <cstdlib>

Ghost::Ghost(int x, int y, int id, GhostStrategy* strategy)
    : Entity(x, y), id(id), strategy(strategy),
      frightened(false), frightenedTimer(0), respawnTimer(0),
      spawnX(x), spawnY(y) {
}

char Ghost::getSymbol() const {
    if (respawnTimer > 0) return ' ';
    return frightened ? 'F' : ('1' + id);
}

int Ghost::getId() const {
    return id;
}

Direction Ghost::chooseDirection(const Game& game) const {
    if (respawnTimer > 0) return Direction::None;
    if (frightened) {
        Direction dirs[] = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
        return dirs[rand() % 4];
    }
    if (strategy) return strategy->chooseDirection(*this, game);
    return Direction::None;
}

bool Ghost::isFrightened() const {
    return frightened;
}

bool Ghost::isRespawning() const {
    return respawnTimer > 0;
}

void Ghost::setFrightened(int ticks) {
    frightened = (ticks > 0);
    frightenedTimer = ticks;
}

void Ghost::updateFrightened() {
    if (respawnTimer > 0) {
        respawnTimer--;
        return;
    }
    if (!frightened) return;
    frightenedTimer--;
    if (frightenedTimer <= 0) {
        frightened = false;
    }
}

void Ghost::respawn() {
    setPosition(spawnX, spawnY);
    frightened = false;
    frightenedTimer = 0;
    respawnTimer = 30;
}
