#include "ConsoleRenderer.h"
#include "Game.h"
#include <iostream>

void ConsoleRenderer::render(Game& game) {
    Map& map = game.getMap();
    Player& player = game.getPlayer();
    std::vector<Ghost>& ghosts = game.getGhosts();
    std::vector<Dot>& dots = game.getDots();

    std::cout << "\033[H";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            bool drawn = false;

            if (x == player.getX() && y == player.getY()) {
                std::cout << 'P';
                drawn = true;
            }

            if (!drawn) {
                for (int i = 0; i < (int)ghosts.size(); i++) {
                    if (x == ghosts[i].getX() && y == ghosts[i].getY()) {
                        std::cout << ghosts[i].getSymbol();
                        drawn = true;
                        break;
                    }
                }
            }
            if (!drawn) {
                for (int i = 0; i < (int)dots.size(); i++) {
                    if (!dots[i].isCollected() && x == dots[i].getX() && y == dots[i].getY()) {
                        std::cout << '.';
                        drawn = true;
                        break;
                    }
                }
            }
            if (!drawn) {
                if (map.isWall(x, y)) {
                    std::cout << '#';
                } else {
                    std::cout << ' ';
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Score: " << player.getScore() << std::endl;
}
