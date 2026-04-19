#ifndef RANDOM_STRATEGY_H
#define RANDOM_STRATEGY_H

#include "GhostStrategy.h"

/// Clyde — porusza sie losowo, ale ucieka gdy gracz jest bardzo blisko (< 4 pola).
class RandomStrategy : public GhostStrategy {
public:
    Direction chooseDirection(const Ghost& ghost, const Game& game) override;
};

#endif
