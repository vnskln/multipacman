#include "AmbusherStrategy.h"
#include "Ghost.h"
#include "Game.h"
#include <cstdlib>

Direction AmbusherStrategy::chooseDirection(const Ghost& ghost, const Game& game) {
    const std::vector<Player>& players = game.getPlayers();

    int closestDist = 9999;
    int closestIdx = -1;

    for (int i = 0; i < (int)players.size(); i++) {
        if (!players[i].isAlive()) continue;
        int dist = abs(players[i].getX() - ghost.getX())
                 + abs(players[i].getY() - ghost.getY());
        if (dist < closestDist) {
            closestDist = dist;
            closestIdx = i;
        }
    }

    if (closestIdx < 0) return Direction::None;

    int targetX = players[closestIdx].getX();
    int targetY = players[closestIdx].getY();

    const Map& map = game.getMap();

    switch (players[closestIdx].getLastMoveDirection()) {
        case Direction::Up:    targetY -= 4; break;
        case Direction::Down:  targetY += 4; break;
        case Direction::Left:  targetX -= 4; break;
        case Direction::Right: targetX += 4; break;
        default: break;
    }

    if (targetX < 0) targetX = 0;
    if (targetY < 0) targetY = 0;
    if (targetX >= map.getWidth()) targetX = map.getWidth() - 1;
    if (targetY >= map.getHeight()) targetY = map.getHeight() - 1;

    int dx = targetX - ghost.getX();
    int dy = targetY - ghost.getY();

    Direction primary, secondary;
    if (abs(dx) >= abs(dy)) {
        primary = (dx > 0) ? Direction::Right : Direction::Left;
        secondary = (dy > 0) ? Direction::Down : Direction::Up;
    } else {
        primary = (dy > 0) ? Direction::Down : Direction::Up;
        secondary = (dx > 0) ? Direction::Right : Direction::Left;
    }

    if (canMove(ghost.getX(), ghost.getY(), primary, map)) return primary;
    if (canMove(ghost.getX(), ghost.getY(), secondary, map)) return secondary;

    return Direction::None;
}
