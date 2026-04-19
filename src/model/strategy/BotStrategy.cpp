#include "BotStrategy.h"
#include "Player.h"
#include "Game.h"
#include <cstdlib>

Direction BotStrategy::chooseDirection(const Player& bot, const Game& game) {
    if (isGhostNearby(bot.getX(), bot.getY(), game, 3)) {
        return fleeDirection(bot, game);
    }

    const std::vector<Dot>& dots = game.getDots();
    const Map& map = game.getMap();

    int closestDist = 9999;
    int targetX = -1;
    int targetY = -1;

    for (int i = 0; i < (int)dots.size(); i++) {
        if (dots[i].isCollected()) continue;
        int dist = abs(dots[i].getX() - bot.getX())
                 + abs(dots[i].getY() - bot.getY());
        if (dist < closestDist) {
            closestDist = dist;
            targetX = dots[i].getX();
            targetY = dots[i].getY();
        }
    }

    if (targetX < 0) return Direction::None;

    int dx = targetX - bot.getX();
    int dy = targetY - bot.getY();

    Direction primary, secondary;
    if (abs(dx) >= abs(dy)) {
        primary = (dx > 0) ? Direction::Right : Direction::Left;
        secondary = (dy > 0) ? Direction::Down : Direction::Up;
    } else {
        primary = (dy > 0) ? Direction::Down : Direction::Up;
        secondary = (dx > 0) ? Direction::Right : Direction::Left;
    }

    if (canMove(bot.getX(), bot.getY(), primary, map)) return primary;
    if (canMove(bot.getX(), bot.getY(), secondary, map)) return secondary;

    Direction dirs[] = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
    int start = rand() % 4;
    for (int i = 0; i < 4; i++) {
        Direction d = dirs[(start + i) % 4];
        if (canMove(bot.getX(), bot.getY(), d, map)) return d;
    }

    return Direction::None;
}

bool BotStrategy::canMove(int x, int y, Direction dir, const Map& map) const {
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

bool BotStrategy::isGhostNearby(int x, int y, const Game& game, int range) const {
    const std::vector<Ghost>& ghosts = game.getGhosts();
    for (int i = 0; i < (int)ghosts.size(); i++) {
        int dist = abs(ghosts[i].getX() - x) + abs(ghosts[i].getY() - y);
        if (dist <= range) return true;
    }
    return false;
}

Direction BotStrategy::fleeDirection(const Player& bot, const Game& game) {
    const std::vector<Ghost>& ghosts = game.getGhosts();
    const Map& map = game.getMap();

    int sumX = 0, sumY = 0, count = 0;
    for (int i = 0; i < (int)ghosts.size(); i++) {
        int dist = abs(ghosts[i].getX() - bot.getX())
                 + abs(ghosts[i].getY() - bot.getY());
        if (dist <= 3) {
            sumX += ghosts[i].getX();
            sumY += ghosts[i].getY();
            count++;
        }
    }

    if (count == 0) return Direction::None;

    int dx = bot.getX() - sumX / count;
    int dy = bot.getY() - sumY / count;

    Direction primary, secondary;
    if (abs(dx) >= abs(dy)) {
        primary = (dx > 0) ? Direction::Right : Direction::Left;
        secondary = (dy > 0) ? Direction::Down : Direction::Up;
    } else {
        primary = (dy > 0) ? Direction::Down : Direction::Up;
        secondary = (dx > 0) ? Direction::Right : Direction::Left;
    }

    if (canMove(bot.getX(), bot.getY(), primary, map)) return primary;
    if (canMove(bot.getX(), bot.getY(), secondary, map)) return secondary;

    Direction dirs[] = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
    for (int i = 0; i < 4; i++) {
        if (canMove(bot.getX(), bot.getY(), dirs[i], map)) return dirs[i];
    }

    return Direction::None;
}
