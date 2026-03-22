#include "Game.h"
#include <cstdlib>

Game::Game() : gameOver(false), playerWon(false) {
    players.push_back(Player(1, 9, 0, "Player1"));
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

            if (x == players[0].getX() && y == players[0].getY()) continue;

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

void Game::handleInput(int playerId, Direction dir) {
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getPlayerId() == playerId) {
            players[i].setDirection(dir);
            return;
        }
    }
}

void Game::update() {
    if (gameOver) return;

    movePlayer();
    collectDots();
    moveGhosts();
    checkCollisions();
    checkWin();
}

void Game::movePlayer() {
    int newX = players[0].getX();
    int newY = players[0].getY();

    switch (players[0].getDirection()) {
        case Direction::Up:    newY--; break;
        case Direction::Down:  newY++; break;
        case Direction::Left:  newX--; break;
        case Direction::Right: newX++; break;
        case Direction::None:  return;
    }

    if (newX >= 0 && newX < map.getWidth() &&
        newY >= 0 && newY < map.getHeight() &&
        !map.isWall(newX, newY)) {
        players[0].setPosition(newX, newY);
    }

    players[0].setDirection(Direction::None);
}

void Game::collectDots() {
    for (int i = 0; i < (int)dots.size(); i++) {
        if (!dots[i].isCollected() &&
            dots[i].getX() == players[0].getX() &&
            dots[i].getY() == players[0].getY()) {
            dots[i].collect();
            players[0].addScore(dots[i].getValue());
        }
    }
}

void Game::moveGhosts() {
    for (int i = 0; i < (int)ghosts.size(); i++) {
        Direction dir = Direction::None;

        if (rand() % 2 == 0) {
            int dx = players[0].getX() - ghosts[i].getX();
            int dy = players[0].getY() - ghosts[i].getY();

            if (abs(dx) > abs(dy)) {
                if (dx > 0) {
                    dir = Direction::Right;
                } else {
                    dir = Direction::Left;
                }
            } else {
                if (dy > 0) {
                    dir = Direction::Down;
                } else {
                    dir = Direction::Up;
                }
            }
        } else {
            int r = rand() % 4;
            switch (r) {
                case 0: dir = Direction::Up; break;
                case 1: dir = Direction::Down; break;
                case 2: dir = Direction::Left; break;
                case 3: dir = Direction::Right; break;
            }
        }

        int newX = ghosts[i].getX();
        int newY = ghosts[i].getY();

        switch (dir) {
            case Direction::Up:    newY--; break;
            case Direction::Down:  newY++; break;
            case Direction::Left:  newX--; break;
            case Direction::Right: newX++; break;
        }

        if (newX >= 0 && newX < map.getWidth() &&
            newY >= 0 && newY < map.getHeight() &&
            !map.isWall(newX, newY)) {
            ghosts[i].setPosition(newX, newY);
        }
    }
}

void Game::checkCollisions() {
    for (int i = 0; i < (int)ghosts.size(); i++) {
        if (ghosts[i].getX() == players[0].getX() &&
            ghosts[i].getY() == players[0].getY()) {
            players[0].kill();
            gameOver = true;
            return;
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

const Map& Game::getMap() const {
    return map;
}

const Player& Game::getPlayer() const {
    return players[0];
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

const std::vector<Ghost>& Game::getGhosts() const {
    return ghosts;
}

const std::vector<Dot>& Game::getDots() const {
    return dots;
}

bool Game::isOver() const {
    return gameOver;
}

bool Game::isWon() const {
    return playerWon;
}
