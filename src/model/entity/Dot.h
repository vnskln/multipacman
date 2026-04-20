#ifndef DOT_H
#define DOT_H

#include "Entity.h"

/// Kropka do zbierania przez graczy. Daje punkty.
class Dot : public Entity {
private:
    bool collected;
    int value;
    bool powerPellet;

public:
    /// @param value ile punktow daje zebranie tej kropki
    Dot(int x, int y, int value = 10, bool powerPellet = false);

    char getSymbol() const override;

    bool isCollected() const;
    int getValue() const;
    bool isPowerPellet() const;
    /// Oznacza kropke jako zebrana.
    void collect();
    void setCollected(bool c);
};

#endif
