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
    Player player;
    std::vector<Ghost> ghosts;
    std::vector<Dot> dots;
    bool gameOver;
    bool playerWon;

    void spawnDots();
    void movePlayer();
    void collectDots();
    void checkWin();

public:
    Game();
    void handleInput(Direction dir);
    void update();

    Map& getMap();
    Player& getPlayer();
    std::vector<Ghost>& getGhosts();
    std::vector<Dot>& getDots();
    bool isOver();
    bool isWon();
};

#endif
