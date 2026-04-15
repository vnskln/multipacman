#include "ChaserStrategy.h"
#include "Ghost.h"
#include "Game.h"
#include <cstdlib>

Direction ChaserStrategy::chooseDirection(const Ghost& ghost, const Game& game) {
    const std::vector<Player>& players = game.getPlayers();

    int closestDist = 9999;
    int targetX = -1;
    int targetY = -1;

    for (int i = 0; i < (int)players.size(); i++) {
        if (!players[i].isAlive()) continue;
        int dist = abs(players[i].getX() - ghost.getX())
                 + abs(players[i].getY() - ghost.getY());
        if (dist < closestDist) {
            closestDist = dist;
            targetX = players[i].getX();
            targetY = players[i].getY();
        }
    }

    if (targetX < 0) return Direction::None;

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

    const Map& map = game.getMap();
    if (canMove(ghost.getX(), ghost.getY(), primary, map)) return primary;
    if (canMove(ghost.getX(), ghost.getY(), secondary, map)) return secondary;

    return Direction::None;
}
