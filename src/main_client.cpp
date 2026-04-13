#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include "GameClient.h"
#include "ConsoleRenderer.h"
#include "Protocol.h"
#include "MainMenuScreen.h"
#include "ConnectScreen.h"
#include "LobbyScreen.h"
#include "GameScreen.h"
#include "Constants.h"

int runConsole() {
    std::string serverIp;
    std::cout << "Server IP: ";
    std::cin >> serverIp;

    std::string name;
    std::cout << "Your name: ";
    std::cin >> name;

    GameClient client;
    if (!client.connect(serverIp, SERVER_PORT, name)) {
        return 1;
    }

    std::cout << "In lobby. Waiting for players..." << std::endl;

    int lastPlayerCount = 0;

    while (client.isConnected() && client.isInLobby()) {
        client.receiveMessages();

        const std::vector<LobbyPlayer>& players = client.getLobbyPlayers();
        if ((int)players.size() != lastPlayerCount) {
            lastPlayerCount = (int)players.size();
            std::cout << "Players in lobby:" << std::endl;
            for (int i = 0; i < (int)players.size(); i++) {
                std::cout << "  " << players[i].name;
                if (players[i].id == 0) std::cout << " (host)";
                std::cout << std::endl;
            }

            if (client.isHost()) {
                std::cout << "Press S to start the game" << std::endl;
            }
        }

        if (client.isHost() && _kbhit()) {
            int key = _getch();
            if (key == 's' || key == 'S') {
                client.sendStartGame();
            }
        }
    }

    if (!client.isConnected()) {
        std::cout << "Disconnected." << std::endl;
        return 1;
    }

    system("cls");

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    ConsoleRenderer renderer;

    while (client.isConnected() && !client.isGameOver()) {
        if (_kbhit()) {
            int key = 0;
            while (_kbhit()) {
                key = _getch();
            }

            std::int32_t dir = DIR_NONE;
            switch (key) {
                case 'w': case 'W': dir = DIR_UP; break;
                case 's': case 'S': dir = DIR_DOWN; break;
                case 'a': case 'A': dir = DIR_LEFT; break;
                case 'd': case 'D': dir = DIR_RIGHT; break;
                case 27:
                    client.disconnect();
                    return 0;
            }

            if (dir != DIR_NONE) {
                client.sendInput(dir);
            }
        }

        client.receiveMessages();

        if (client.isGameStarted()) {
            renderer.render(client.getGame());
        }
    }

    std::cout << std::endl << "Game over! Press any key..." << std::endl;
    _getch();
    client.disconnect();
    return 0;
}

int runGui() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "MULTIPACMAN",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Cannot load font" << std::endl;
        return 1;
    }

    GameClient client;

    enum class AppState { Menu, Connect, Lobby, Game };
    AppState state = AppState::Menu;

    MainMenuScreen menuScreen(font);
    ConnectScreen connectScreen(font);
    LobbyScreen lobbyScreen(font, client);
    GameScreen gameScreen(font, window, client);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                continue;
            }

            switch (state) {
                case AppState::Menu:
                    menuScreen.handleEvent(*event);
                    break;
                case AppState::Connect:
                    connectScreen.handleEvent(*event);
                    break;
                case AppState::Lobby:
                    lobbyScreen.handleEvent(*event);
                    break;
                case AppState::Game:
                    gameScreen.handleEvent(*event);
                    break;
            }
        }

        switch (state) {
            case AppState::Menu:
                menuScreen.update();
                if (menuScreen.wantsExit()) {
                    window.close();
                } else if (menuScreen.wantsConnect()) {
                    state = AppState::Connect;
                    menuScreen.reset();
                    connectScreen.reset();
                }
                break;

            case AppState::Connect:
                connectScreen.update();
                if (connectScreen.wantsBack()) {
                    state = AppState::Menu;
                    connectScreen.reset();
                } else if (connectScreen.wantsConnect()) {
                    std::string name = connectScreen.getName();
                    connectScreen.clearConnect();

                    if (name.empty()) {
                        connectScreen.setError("Name cannot be empty");
                    } else if (client.connect(connectScreen.getIp(),
                                              connectScreen.getPort(),
                                              name)) {
                        state = AppState::Lobby;
                        lobbyScreen.reset();
                    } else {
                        connectScreen.setError("Could not connect to server");
                    }
                }
                break;

            case AppState::Lobby:
                client.receiveMessages();
                lobbyScreen.update();

                if (lobbyScreen.wantsDisconnect()) {
                    client.disconnect();
                    state = AppState::Menu;
                    menuScreen.reset();
                } else if (!client.isConnected()) {
                    state = AppState::Menu;
                    menuScreen.reset();
                } else if (client.isGameStarted()) {
                    state = AppState::Game;
                    gameScreen.reset();
                }
                break;

            case AppState::Game:
                client.receiveMessages();
                gameScreen.update();

                if (gameScreen.wantsMenu()) {
                    client.disconnect();
                    state = AppState::Menu;
                    menuScreen.reset();
                } else if (!client.isConnected() && !client.isGameOver()) {
                    state = AppState::Menu;
                    menuScreen.reset();
                }
                break;
        }

        window.clear();
        switch (state) {
            case AppState::Menu:
                menuScreen.draw(window);
                break;
            case AppState::Connect:
                connectScreen.draw(window);
                break;
            case AppState::Lobby:
                lobbyScreen.draw(window);
                break;
            case AppState::Game:
                gameScreen.draw(window);
                break;
        }
        window.display();
    }

    if (client.isConnected()) {
        client.disconnect();
    }

    return 0;
}

int main(int argc, char* argv[]) {
    bool useGui = true;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--console") useGui = false;
    }

    if (useGui) return runGui();
    return runConsole();
}
