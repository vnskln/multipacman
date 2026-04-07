#include "Game.h"
#include <cstdlib>

Game::Game() : gameOver(false), playerWon(false) {
    ghosts.push_back(Ghost(6, 5, 0));
    ghosts.push_back(Ghost(7, 5, 1));
    ghosts.push_back(Ghost(8, 5, 2));
    ghosts.push_back(Ghost(9, 5, 3));
}

void Game::addPlayer(int playerId, const std::string& name) {
    int startX = 1;
    int startY = 1;
    if (playerId == 1) { startX = 13; startY = 1; }
    if (playerId == 2) { startX = 1;  startY = 9; }
    if (playerId == 3) { startX = 13; startY = 9; }
    players.push_back(Player(startX, startY, playerId, name));
}

void Game::removePlayer(int playerId) {
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getPlayerId() == playerId) {
            players[i].disconnect();
            return;
        }
    }
}

void Game::start() {
    spawnDots();
}

void Game::spawnDots() {
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.isWall(x, y)) continue;

            bool isPlayerSpawn = false;
            for (int i = 0; i < (int)players.size(); i++) {
                if (x == players[i].getX() && y == players[i].getY()) {
                    isPlayerSpawn = true;
                    break;
                }
            }
            if (isPlayerSpawn) continue;

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

    movePlayers();
    collectDots();
    moveGhosts();
    checkCollisions();
    checkWin();
}

void Game::movePlayers() {
    for (int p = 0; p < (int)players.size(); p++) {
        if (!players[p].isAlive()) continue;

        int newX = players[p].getX();
        int newY = players[p].getY();

        switch (players[p].getDirection()) {
            case Direction::Up:    newY--; break;
            case Direction::Down:  newY++; break;
            case Direction::Left:  newX--; break;
            case Direction::Right: newX++; break;
            case Direction::None:  continue;
        }

        if (newX >= 0 && newX < map.getWidth() &&
            newY >= 0 && newY < map.getHeight() &&
            !map.isWall(newX, newY)) {
            players[p].setPosition(newX, newY);
        }

        players[p].setDirection(Direction::None);
    }
}

void Game::collectDots() {
    for (int p = 0; p < (int)players.size(); p++) {
        if (!players[p].isAlive()) continue;

        for (int i = 0; i < (int)dots.size(); i++) {
            if (!dots[i].isCollected() &&
                dots[i].getX() == players[p].getX() &&
                dots[i].getY() == players[p].getY()) {
                dots[i].collect();
                players[p].addScore(dots[i].getValue());
            }
        }
    }
}

void Game::moveGhosts() {
    for (int i = 0; i < (int)ghosts.size(); i++) {
        Direction dir = Direction::None;

        int closestPlayer = -1;
        int closestDist = 9999;
        for (int p = 0; p < (int)players.size(); p++) {
            if (!players[p].isAlive()) continue;
            int dist = abs(players[p].getX() - ghosts[i].getX())
                     + abs(players[p].getY() - ghosts[i].getY());
            if (dist < closestDist) {
                closestDist = dist;
                closestPlayer = p;
            }
        }

        if (closestPlayer >= 0 && rand() % 2 == 0) {
            int dx = players[closestPlayer].getX() - ghosts[i].getX();
            int dy = players[closestPlayer].getY() - ghosts[i].getY();

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
    for (int p = 0; p < (int)players.size(); p++) {
        if (!players[p].isAlive()) continue;

        for (int i = 0; i < (int)ghosts.size(); i++) {
            if (ghosts[i].getX() == players[p].getX() &&
                ghosts[i].getY() == players[p].getY()) {
                players[p].kill();
                break;
            }
        }
    }
}

void Game::checkWin() {
    bool allCollected = true;
    for (int i = 0; i < (int)dots.size(); i++) {
        if (!dots[i].isCollected()) {
            allCollected = false;
            break;
        }
    }
    if (allCollected) {
        gameOver = true;
        playerWon = true;
        return;
    }

    int aliveCount = 0;
    for (int p = 0; p < (int)players.size(); p++) {
        if (players[p].isAlive()) aliveCount++;
    }
    if (aliveCount == 0) {
        gameOver = true;
    }
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

std::vector<Player>& Game::getPlayers() {
    return players;
}

std::vector<Ghost>& Game::getGhosts() {
    return ghosts;
}

std::vector<Dot>& Game::getDots() {
    return dots;
}

void Game::setOver(bool over, bool won) {
    gameOver = over;
    playerWon = won;
}

bool Game::isOver() const {
    return gameOver;
}

bool Game::isWon() const {
    return playerWon;
}
