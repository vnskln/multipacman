#include "Player.h"

Player::Player(int x, int y, int playerId, const std::string& name)
    : Entity(x, y), playerId(playerId), name(name), score(0), state(State::Alive) {
}

char Player::getSymbol() const {
    return 'P';
}

int Player::getPlayerId() const {
    return playerId;
}

std::string Player::getName() const {
    return name;
}

Player::State Player::getState() const {
    return state;
}

int Player::getScore() const {
    return score;
}

bool Player::isAlive() const {
    return state == State::Alive;
}

void Player::addScore(int points) {
    score += points;
}

void Player::kill() {
    state = State::Dead;
}

void Player::disconnect() {
    state = State::Disconnected;
}
