#ifndef GHOST_H
#define GHOST_H

#include "Entity.h"

class GhostStrategy;
class Game;

/// Duch scigajacy graczy. Porusza sie co drugi tick.
/// Kazdy duch ma wlasna strategie ruchu (Strategy pattern).
class Ghost : public Entity {
private:
    int id;
    GhostStrategy* strategy;
    bool frightened;
    int frightenedTimer;
    int respawnTimer;
    int spawnX;
    int spawnY;

public:
    Ghost(int x, int y, int id, GhostStrategy* strategy = nullptr);

    char getSymbol() const override;

    int getId() const;

    /// Wybiera kierunek ruchu korzystajac ze swojej strategii.
    Direction chooseDirection(const Game& game) const;

    bool isFrightened() const;
    bool isRespawning() const;
    void setFrightened(int ticks);
    void updateFrightened();
    void respawn();
};

#endif
