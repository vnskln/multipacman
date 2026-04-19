#ifndef ENTITY_H
#define ENTITY_H

/// Kierunek ruchu na planszy.
enum class Direction { Up, Down, Left, Right, None };

/**
 * Klasa bazowa dla obiektow na planszy.
 * Kazdy obiekt ma pozycje (x, y) i kierunek ruchu.
 */
class Entity {
protected:
    int x;
    int y;
    Direction direction;

public:
    Entity(int x, int y);
    virtual ~Entity();

    /// Zwraca znak reprezentujacy obiekt w trybie konsolowym.
    virtual char getSymbol() const = 0;

    int getX() const;
    int getY() const;
    Direction getDirection() const;
    void setDirection(Direction dir);
    void setPosition(int newX, int newY);
};

#endif
