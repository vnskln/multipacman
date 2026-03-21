#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
private:
    int score;
    bool alive;

public:
    Player(int x, int y);

    char getSymbol() const override;

    int getScore() const;
    bool isAlive() const;
    void addScore(int points);
    void kill();
};

#endif
