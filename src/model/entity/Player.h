#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>

/// Gracz sterowany przez czlowieka przez siec lub bot AI.
class Player : public Entity {
public:
    /// Stan gracza w rozgrywce.
    enum class State { Alive, Dead, Disconnected };

private:
    int playerId;
    std::string name;
    int score;
    State state;
    Direction lastMoveDirection;
    bool bot;

public:
    Player(int x, int y, int playerId = 0, const std::string& name = "", bool bot = false);

    char getSymbol() const override;

    int getPlayerId() const;
    const std::string& getName() const;
    State getState() const;
    int getScore() const;
    bool isAlive() const;
    void addScore(int points);
    /// Zabija gracza (zmiana stanu na Dead).
    void kill();
    /// Oznacza gracza jako rozlaczonego.
    void disconnect();
    void setScore(int s);
    void setState(State s);

    /// Ostatni kierunek w jakim gracz sie ruszyl (uzywany przez AmbusherStrategy).
    Direction getLastMoveDirection() const;
    void setLastMoveDirection(Direction dir);

    /// Czy gracz jest botem AI.
    bool isBot() const;
};

#endif
