#ifndef AMBUSHER_STRATEGY_H
#define AMBUSHER_STRATEGY_H

#include "GhostStrategy.h"

/// Pinky — celuje 4 pola przed graczem w kierunku jego ruchu.
class AmbusherStrategy : public GhostStrategy {
public:
    Direction chooseDirection(const Ghost& ghost, const Game& game) override;
};

#endif
