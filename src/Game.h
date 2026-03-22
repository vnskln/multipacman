#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include "Ghost.h"
#include "Dot.h"
#include <vector>

class Game {
private:
    Map map;
    std::vector<Player> players;
    std::vector<Ghost> ghosts;
    std::vector<Dot> dots;
    bool gameOver;
    bool playerWon;

    void spawnDots();
    void movePlayers();
    void moveGhosts();
    void collectDots();
    void checkCollisions();
    void checkWin();

public:
    Game();
    void addPlayer(int playerId, const std::string& name);
    void removePlayer(int playerId);
    void start();
    void handleInput(int playerId, Direction dir);
    void update();

    const Map& getMap() const;
    const Player& getPlayer() const;
    const std::vector<Player>& getPlayers() const;
    const std::vector<Ghost>& getGhosts() const;
    const std::vector<Dot>& getDots() const;

    std::vector<Player>& getPlayers();
    std::vector<Ghost>& getGhosts();
    std::vector<Dot>& getDots();
    void setOver(bool over, bool won);

    bool isOver() const;
    bool isWon() const;
};

#endif
