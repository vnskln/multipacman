#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>

class Player : public Entity {
public:
    enum class State { Alive, Dead, Disconnected };

private:
    int playerId;
    std::string name;
    int score;
    State state;

public:
    Player(int x, int y, int playerId = 0, const std::string& name = "");

    char getSymbol() const override;

    int getPlayerId() const;
    std::string getName() const;
    State getState() const;
    int getScore() const;
    bool isAlive() const;
    void addScore(int points);
    void kill();
    void disconnect();
};

#endif
