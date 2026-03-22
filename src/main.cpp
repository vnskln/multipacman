#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "Game.h"
#include "ConsoleRenderer.h"

int main() {
    srand(time(nullptr));

    Game game;
    ConsoleRenderer renderer;

    system("cls");

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    bool running = true;

    while (running && !game.isOver()) {
        if (_kbhit()) {
            int key = 0;
            while (_kbhit()) {
                key = _getch();
            }

            switch (key) {
                case 'w':
                case 'W':
                    game.handleInput(0, Direction::Up);
                    break;
                case 's':
                case 'S':
                    game.handleInput(0, Direction::Down);
                    break;
                case 'a':
                case 'A':
                    game.handleInput(0, Direction::Left);
                    break;
                case 'd':
                case 'D':
                    game.handleInput(0, Direction::Right);
                    break;
                case 27:
                    running = false;
                    break;
            }
        }

        game.update();
        renderer.render(game);
        Sleep(225);
    }

    renderer.render(game);

    if (game.isWon()) {
        std::cout << "YOU WIN! Score: " << game.getPlayer().getScore() << std::endl;
    } else if (game.isOver()) {
        std::cout << "GAME OVER! Score: " << game.getPlayer().getScore() << std::endl;
    }

    std::cout << "Press any key to exit..." << std::endl;
    _getch();

    return 0;
}
