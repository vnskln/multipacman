#include <iostream>
#include "Game.h"

int main() {
    Game game;

    std::cout << "MULTIPACMAN" << std::endl;
    std::cout << "Dots: " << game.getDots().size() << std::endl;
    std::cout << "Ghosts: " << game.getGhosts().size() << std::endl;
    std::cout << "Player at (" << game.getPlayer().getX() << "," << game.getPlayer().getY() << ")" << std::endl;

    game.handleInput(Direction::Right);
    game.update();
    game.update();
    game.update();

    std::cout << "After 3 updates:" << std::endl;
    std::cout << "Player at (" << game.getPlayer().getX() << "," << game.getPlayer().getY() << ")" << std::endl;
    std::cout << "Score: " << game.getPlayer().getScore() << std::endl;
    std::cout << "Game over: " << (game.isOver() ? "yes" : "no") << std::endl;

    return 0;
}
