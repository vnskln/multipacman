#include <iostream>
#include <cstdlib>
#include "Game.h"
#include "ConsoleRenderer.h"

int main() {
    Game game;
    ConsoleRenderer renderer;

    system("cls");
    renderer.render(game);

    game.handleInput(Direction::Right);
    game.update();
    game.update();
    game.update();

    renderer.render(game);

    return 0;
}
