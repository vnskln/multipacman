#include "RandomStrategy.h"
#include "Ghost.h"
#include "Game.h"
#include <cstdlib>

Direction RandomStrategy::chooseDirection(const Ghost& ghost, const Game& game) {
    const std::vector<Player>& players = game.getPlayers();
    const Map& map = game.getMap();

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

    if (closestIdx >= 0 && closestDist < 4) {
        int dx = ghost.getX() - players[closestIdx].getX();
        int dy = ghost.getY() - players[closestIdx].getY();

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
    }

    Direction dirs[] = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
    int start = rand() % 4;
    for (int i = 0; i < 4; i++) {
        Direction dir = dirs[(start + i) % 4];
        if (canMove(ghost.getX(), ghost.getY(), dir, map)) return dir;
    }

    return Direction::None;
}
