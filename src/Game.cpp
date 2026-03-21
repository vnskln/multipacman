#include "Game.h"

Game::Game() : player(1, 9), gameOver(false), playerWon(false) {
    ghosts.push_back(Ghost(6, 5, 0));
    ghosts.push_back(Ghost(7, 5, 1));
    ghosts.push_back(Ghost(8, 5, 2));
    ghosts.push_back(Ghost(9, 5, 3));
    spawnDots();
}

void Game::spawnDots() {
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.isWall(x, y)) continue;

            if (x == player.getX() && y == player.getY()) continue;

            bool isGhostSpawn = false;
            for (int i = 0; i < (int)ghosts.size(); i++) {
                if (x == ghosts[i].getX() && y == ghosts[i].getY()) {
                    isGhostSpawn = true;
                    break;
                }
            }
            if (isGhostSpawn) continue;

            dots.push_back(Dot(x, y));
        }
    }
}

void Game::handleInput(Direction dir) {
    player.setDirection(dir);
}

void Game::update() {
    if (gameOver) return;

    movePlayer();
    collectDots();
    checkWin();
}

void Game::movePlayer() {
    int newX = player.getX();
    int newY = player.getY();

    switch (player.getDirection()) {
        case Direction::Up:    newY--; break;
        case Direction::Down:  newY++; break;
        case Direction::Left:  newX--; break;
        case Direction::Right: newX++; break;
        case Direction::None:  return;
    }

    if (newX >= 0 && newX < map.getWidth() &&
        newY >= 0 && newY < map.getHeight() &&
        !map.isWall(newX, newY)) {
        player.setPosition(newX, newY);
    }
}

void Game::collectDots() {
    for (int i = 0; i < (int)dots.size(); i++) {
        if (!dots[i].isCollected() &&
            dots[i].getX() == player.getX() &&
            dots[i].getY() == player.getY()) {
            dots[i].collect();
            player.addScore(dots[i].getValue());
        }
    }
}

void Game::checkWin() {
    for (int i = 0; i < (int)dots.size(); i++) {
        if (!dots[i].isCollected()) return;
    }
    gameOver = true;
    playerWon = true;
}

Map& Game::getMap() {
    return map;
}

Player& Game::getPlayer() {
    return player;
}

std::vector<Ghost>& Game::getGhosts() {
    return ghosts;
}

std::vector<Dot>& Game::getDots() {
    return dots;
}

bool Game::isOver() {
    return gameOver;
}

bool Game::isWon() {
    return playerWon;
}
