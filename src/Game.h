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
    void movePlayer();
    void moveGhosts();
    void collectDots();
    void checkCollisions();
    void checkWin();

public:
    Game();
    void handleInput(int playerId, Direction dir);
    void update();

    const Map& getMap() const;
    const Player& getPlayer() const;
    const std::vector<Player>& getPlayers() const;
    const std::vector<Ghost>& getGhosts() const;
    const std::vector<Dot>& getDots() const;
    bool isOver() const;
    bool isWon() const;
};

#endif
