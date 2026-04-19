#ifndef CHASER_STRATEGY_H
#define CHASER_STRATEGY_H

#include "GhostStrategy.h"

/// Blinky — zawsze goni najblizszego gracza najkrotsza droga.
class ChaserStrategy : public GhostStrategy {
public:
    Direction chooseDirection(const Ghost& ghost, const Game& game) override;
};

#endif
