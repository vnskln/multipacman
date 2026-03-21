#include <iostream>
#include "Map.h"
#include "Player.h"
#include "Ghost.h"
#include "Dot.h"

int main() {
    Map map;
    Player player(1, 1);
    Ghost ghost(7, 5, 0);
    Dot dot(3, 3);

    std::cout << player.getSymbol() << " at (" << player.getX() << "," << player.getY() << ")" << std::endl;
    std::cout << ghost.getSymbol() << " at (" << ghost.getX() << "," << ghost.getY() << ")" << std::endl;
    std::cout << dot.getSymbol() << " at (" << dot.getX() << "," << dot.getY() << ")" << std::endl;

    std::cout << std::endl;
    std::cout << "MULTIPACMAN " << map.getWidth() << "x" << map.getHeight() << std::endl;
    std::cout << std::endl;

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.isWall(x, y)) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
