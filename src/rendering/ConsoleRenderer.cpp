#include "ConsoleRenderer.h"
#include "Game.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    const Map& map = game.getMap();
    const std::vector<Player>& players = game.getPlayers();
    const std::vector<Ghost>& ghosts = game.getGhosts();
    const std::vector<Dot>& dots = game.getDots();

    std::cout << "\033[H";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            bool drawn = false;

            for (int p = 0; p < (int)players.size(); p++) {
                if (players[p].isAlive() &&
                    x == players[p].getX() && y == players[p].getY()) {
                    std::cout << players[p].getSymbol();
                    drawn = true;
                    break;
                }
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

    for (int p = 0; p < (int)players.size(); p++) {
        std::cout << players[p].getName() << ": " << players[p].getScore();
        if (!players[p].isAlive()) std::cout << " [DEAD]";
        std::cout << "  ";
    }
    std::cout << std::endl;
}
