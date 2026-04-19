#ifndef GHOST_STRATEGY_H
#define GHOST_STRATEGY_H

#include "Entity.h"
#include "Map.h"

class Ghost;
class Game;

/// Abstrakcyjna strategia ruchu ducha.
class GhostStrategy {
public:
    virtual ~GhostStrategy() {}
    /// Wybiera kierunek ruchu dla ducha na podstawie stanu gry.
    virtual Direction chooseDirection(const Ghost& ghost, const Game& game) = 0;

protected:
    /// Sprawdza czy ruch w danym kierunku nie wychodzi za mape i nie trafia w sciane.
    bool canMove(int x, int y, Direction dir, const Map& map) const {
        int nx = x, ny = y;
        switch (dir) {
            case Direction::Up:    ny--; break;
            case Direction::Down:  ny++; break;
            case Direction::Left:  nx--; break;
            case Direction::Right: nx++; break;
            default: return false;
        }
        return nx >= 0 && nx < map.getWidth() &&
               ny >= 0 && ny < map.getHeight() &&
               !map.isWall(nx, ny);
    }
};

#endif
