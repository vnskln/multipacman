#include "Entity.h"

Entity::Entity(int x, int y) : x(x), y(y), direction(Direction::None) {
}

Entity::~Entity() {
}

int Entity::getX() const {
    return x;
}

int Entity::getY() const {
    return y;
}

Direction Entity::getDirection() const {
    return direction;
}

void Entity::setDirection(Direction dir) {
    direction = dir;
}

void Entity::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}
