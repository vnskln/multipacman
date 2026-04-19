#ifndef PATROL_STRATEGY_H
#define PATROL_STRATEGY_H

#include "GhostStrategy.h"

/// Inky — patroluje losowo, ale goni gracza gdy ten sie zbliza (< 8 pol).
class PatrolStrategy : public GhostStrategy {
public:
    Direction chooseDirection(const Ghost& ghost, const Game& game) override;
};

#endif
