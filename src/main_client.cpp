#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include "GameClient.h"
#include "ConsoleRenderer.h"
#include "Protocol.h"

int main() {
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
