#include "Game.h"
#include "ChaserStrategy.h"
#include "AmbusherStrategy.h"
#include "PatrolStrategy.h"
#include "RandomStrategy.h"
#include <cstdlib>
#include <iostream>

Game::Game() : gameOver(false), playerWon(false), tickCount(0), ghostTickDivisor(2), stuckTicks(0) {
}

Game::Game(const std::string& mapFile) : gameOver(false), playerWon(false), tickCount(0), ghostTickDivisor(2), stuckTicks(0) {
    if (!map.loadFromFile(mapFile)) {
        std::cerr << "Failed to load map: " << mapFile << std::endl;
    }
    createGhosts();
}

Game::Game(const std::vector<std::string>& mapLines, const std::string& mapName)
    : gameOver(false), playerWon(false), tickCount(0), ghostTickDivisor(2), stuckTicks(0) {
    map.loadFromLines(mapLines, mapName);
}

void Game::createGhosts() {
    const std::vector<SpawnPoint>& spawns = map.getGhostSpawns();
    for (int i = 0; i < (int)spawns.size(); i++) {
        switch (i % 4) {
            case 0: strategies.push_back(std::make_unique<ChaserStrategy>()); break;
            case 1: strategies.push_back(std::make_unique<AmbusherStrategy>()); break;
            case 2: strategies.push_back(std::make_unique<PatrolStrategy>()); break;
            case 3: strategies.push_back(std::make_unique<RandomStrategy>()); break;
        }
        ghosts.push_back(Ghost(spawns[i].x, spawns[i].y, i, strategies[i].get()));
    }
}

void Game::addPlayer(int playerId, const std::string& name) {
    if ((int)players.size() >= 4) return;
    const std::vector<SpawnPoint>& spawns = map.getPlayerSpawns();
    int idx = (int)players.size();
    int startX = 1, startY = 1;
    if (idx < (int)spawns.size()) {
        startX = spawns[idx].x;
        startY = spawns[idx].y;
    }
    players.push_back(Player(startX, startY, playerId, name));
}

void Game::addBot(int playerId, const std::string& name) {
    if ((int)players.size() >= 4) return;
    const std::vector<SpawnPoint>& spawns = map.getPlayerSpawns();
    int idx = (int)players.size();
    int startX = 1, startY = 1;
    if (idx < (int)spawns.size()) {
        startX = spawns[idx].x;
        startY = spawns[idx].y;
    }
    players.push_back(Player(startX, startY, playerId, name, true));
}

void Game::addGhost(int id, int x, int y) {
    ghosts.push_back(Ghost(x, y, id, nullptr));
}

void Game::addDot(int x, int y, bool powerPellet) {
    int value = powerPellet ? 50 : 10;
    dots.push_back(Dot(x, y, value, powerPellet));
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

            bool isPelletSpawn = false;
            const std::vector<SpawnPoint>& pellets = map.getPelletSpawns();
            for (int i = 0; i < (int)pellets.size(); i++) {
                if (x == pellets[i].x && y == pellets[i].y) {
                    isPelletSpawn = true;
                    break;
                }
            }

            if (isPelletSpawn) {
                dots.push_back(Dot(x, y, 50, true));
            } else {
                dots.push_back(Dot(x, y));
            }
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

    tickCount++;
    stuckTicks++;

    movePlayers();
    collectDots();

    if (tickCount % ghostTickDivisor == 0) {
        moveBots();
        moveGhosts();
    }

    for (int i = 0; i < (int)ghosts.size(); i++) {
        ghosts[i].updateFrightened();
    }

    checkCollisions();
    checkWin();
}

void Game::moveBots() {
    for (int p = 0; p < (int)players.size(); p++) {
        if (!players[p].isBot()) continue;
        if (!players[p].isAlive()) continue;
        Direction dir = botStrategy.chooseDirection(players[p], *this);
        players[p].setDirection(dir);
    }
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
            default: continue;
        }

        if (newX >= 0 && newX < map.getWidth() &&
            newY >= 0 && newY < map.getHeight() &&
            !map.isWall(newX, newY)) {
            players[p].setLastMoveDirection(players[p].getDirection());
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
                stuckTicks = 0;
                players[p].addScore(dots[i].getValue());

                if (dots[i].isPowerPellet()) {
                    for (int g = 0; g < (int)ghosts.size(); g++) {
                        ghosts[g].setFrightened(50);
                    }
                }
            }
        }
    }
}

void Game::moveGhosts() {
    for (int i = 0; i < (int)ghosts.size(); i++) {
        Direction dir = ghosts[i].chooseDirection(*this);

        int newX = ghosts[i].getX();
        int newY = ghosts[i].getY();

        switch (dir) {
            case Direction::Up:    newY--; break;
            case Direction::Down:  newY++; break;
            case Direction::Left:  newX--; break;
            case Direction::Right: newX++; break;
            default: continue;
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
            if (ghosts[i].isRespawning()) continue;
            if (ghosts[i].getX() == players[p].getX() &&
                ghosts[i].getY() == players[p].getY()) {
                if (ghosts[i].isFrightened()) {
                    ghosts[i].respawn();
                    players[p].addScore(200);
                } else {
                    players[p].kill();
                }
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

    int aliveAny = 0;
    for (int p = 0; p < (int)players.size(); p++) {
        if (players[p].isAlive()) aliveAny++;
    }
    if (aliveAny == 0) {
        gameOver = true;
        return;
    }

    if (stuckTicks > 300) {
        gameOver = true;
    }
}

const Map& Game::getMap() const {
    return map;
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

void Game::setGhostTickDivisor(int n) {
    if (n < 1) n = 1;
    ghostTickDivisor = n;
}

bool Game::isOver() const {
    return gameOver;
}

bool Game::isWon() const {
    return playerWon;
}
