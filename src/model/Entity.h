#ifndef ENTITY_H
#define ENTITY_H

enum class Direction { Up, Down, Left, Right, None };

class Entity {
protected:
    int x;
    int y;
    Direction direction;

public:
    Entity(int x, int y);
    virtual ~Entity();

    virtual char getSymbol() const = 0;

    int getX() const;
    int getY() const;
    Direction getDirection() const;
    void setDirection(Direction dir);
    void setPosition(int newX, int newY);
};

#endif
