#ifndef DOT_H
#define DOT_H

#include "Entity.h"

class Dot : public Entity {
private:
    bool collected;
    int value;

public:
    Dot(int x, int y, int value = 10);

    char getSymbol() const override;

    bool isCollected() const;
    int getValue() const;
    void collect();
    void setCollected(bool c);
};

#endif
