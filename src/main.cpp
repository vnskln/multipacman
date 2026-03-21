#include <iostream>
#include "Map.h"

int main() {
    Map map;
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
