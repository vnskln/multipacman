#include "Player.h"

Player::Player(int x, int y, int playerId, const std::string& name, bool bot)
    : Entity(x, y), playerId(playerId), name(name), score(0), state(State::Alive),
      lastMoveDirection(Direction::None), bot(bot) {
}

char Player::getSymbol() const {
    return 'A' + playerId;
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

void Player::setScore(int s) {
    score = s;
}

void Player::setState(State s) {
    state = s;
}

Direction Player::getLastMoveDirection() const {
    return lastMoveDirection;
}

void Player::setLastMoveDirection(Direction dir) {
    lastMoveDirection = dir;
}

bool Player::isBot() const {
    return bot;
}
