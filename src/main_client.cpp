#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include "Game.h"
#include "ConsoleRenderer.h"
#include "NetworkDefs.h"

int main() {
    std::string serverIp;
    std::cout << "Server IP: ";
    std::cin >> serverIp;

    auto ip = sf::IpAddress::resolve(serverIp);
    if (!ip) {
        std::cerr << "Invalid IP" << std::endl;
        return 1;
    }

    sf::TcpSocket socket;
    std::cout << "Connecting..." << std::endl;

    if (socket.connect(*ip, SERVER_PORT, sf::seconds(5)) != sf::Socket::Status::Done) {
        std::cerr << "Cannot connect" << std::endl;
        return 1;
    }
    std::cout << "Connected!" << std::endl;

    Game game;
    game.addPlayer(0, "Player");
    game.start();

    ConsoleRenderer renderer;

    system("cls");

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    sf::SocketSelector selector;
    selector.add(socket);

    bool running = true;

    while (running) {
        if (_kbhit()) {
            int key = 0;
            while (_kbhit()) {
                key = _getch();
            }

            std::int32_t dir = -1;
            switch (key) {
                case 'w': case 'W': dir = 0; break;
                case 's': case 'S': dir = 1; break;
                case 'a': case 'A': dir = 2; break;
                case 'd': case 'D': dir = 3; break;
                case 27: running = false; break;
            }

            if (dir >= 0) {
                sf::Packet packet;
                packet << dir;
                (void)socket.send(packet);
            }
        }

        if (selector.wait(sf::milliseconds(50))) {
            if (selector.isReady(socket)) {
                sf::Packet packet;
                sf::Socket::Status status = socket.receive(packet);

                if (status == sf::Socket::Status::Disconnected) {
                    std::cout << "Server disconnected" << std::endl;
                    break;
                }

                if (status == sf::Socket::Status::Done) {
                    std::int32_t playerCount;
                    packet >> playerCount;

                    std::vector<Player>& players = game.getPlayers();
                    for (int i = 0; i < playerCount; i++) {
                        std::int32_t id, x, y, score, state;
                        packet >> id >> x >> y >> score >> state;
                        players[i].setPosition(x, y);
                        players[i].setScore(score);

                        Player::State ps = Player::State::Alive;
                        if (state == 0) ps = Player::State::Alive;
                        if (state == 1) ps = Player::State::Dead;
                        if (state == 2) ps = Player::State::Disconnected;
                        players[i].setState(ps);
                    }

                    std::int32_t ghostCount;
                    packet >> ghostCount;

                    std::vector<Ghost>& ghosts = game.getGhosts();
                    for (int i = 0; i < ghostCount; i++) {
                        std::int32_t id, x, y;
                        packet >> id >> x >> y;
                        ghosts[i].setPosition(x, y);
                    }

                    std::int32_t dotCount;
                    packet >> dotCount;

                    std::vector<Dot>& dots = game.getDots();
                    for (int i = 0; i < dotCount; i++) {
                        std::int32_t x, y, collected;
                        packet >> x >> y >> collected;
                        dots[i].setCollected(collected == 1);
                    }

                    std::int32_t over;
                    packet >> over;
                    game.setOver(over == 1, false);

                    renderer.render(game);

                    if (over == 1) running = false;
                }
            }
        }
    }

    std::cout << "Game over! Press any key..." << std::endl;
    _getch();
    socket.disconnect();
    return 0;
}
