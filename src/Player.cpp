#include "Player.h"

Player::Player(int x, int y) : Entity(x, y), score(0), alive(true) {
}

char Player::getSymbol() const {
    return 'P';
}

int Player::getScore() const {
    return score;
}

bool Player::isAlive() const {
    return alive;
}

void Player::addScore(int points) {
    score += points;
}

void Player::kill() {
    alive = false;
}
