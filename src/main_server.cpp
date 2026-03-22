#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "NetworkDefs.h"

int main() {
    srand(time(nullptr));

    sf::TcpListener listener;
    if (listener.listen(SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Cannot listen on port " << SERVER_PORT << std::endl;
        return 1;
    }
    std::cout << "Server started on port " << SERVER_PORT << std::endl;
    std::cout << "Waiting for player..." << std::endl;

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Status::Done) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }
    std::cout << "Client connected!" << std::endl;

    Game game;
    game.addPlayer(0, "Player");
    game.start();

    sf::SocketSelector selector;
    selector.add(client);

    while (!game.isOver()) {
        if (selector.wait(sf::milliseconds(200))) {
            if (selector.isReady(client)) {
                sf::Packet packet;
                sf::Socket::Status status = client.receive(packet);

                if (status == sf::Socket::Status::Disconnected) {
                    std::cout << "Client disconnected" << std::endl;
                    break;
                }

                if (status == sf::Socket::Status::Done) {
                    std::int32_t dir;
                    packet >> dir;

                    Direction direction = Direction::None;
                    if (dir == 0) direction = Direction::Up;
                    if (dir == 1) direction = Direction::Down;
                    if (dir == 2) direction = Direction::Left;
                    if (dir == 3) direction = Direction::Right;

                    game.handleInput(0, direction);
                }
            }
        }

        game.update();

        const std::vector<Player>& players = game.getPlayers();
        const std::vector<Ghost>& ghosts = game.getGhosts();
        const std::vector<Dot>& dots = game.getDots();

        sf::Packet state;
        state << (std::int32_t)players.size();
        for (int i = 0; i < (int)players.size(); i++) {
            state << (std::int32_t)players[i].getPlayerId()
                  << (std::int32_t)players[i].getX()
                  << (std::int32_t)players[i].getY()
                  << (std::int32_t)players[i].getScore()
                  << (std::int32_t)players[i].getState();
        }

        state << (std::int32_t)ghosts.size();
        for (int i = 0; i < (int)ghosts.size(); i++) {
            state << (std::int32_t)ghosts[i].getId()
                  << (std::int32_t)ghosts[i].getX()
                  << (std::int32_t)ghosts[i].getY();
        }

        state << (std::int32_t)dots.size();
        for (int i = 0; i < (int)dots.size(); i++) {
            state << (std::int32_t)dots[i].getX()
                  << (std::int32_t)dots[i].getY()
                  << (std::int32_t)(dots[i].isCollected() ? 1 : 0);
        }

        state << (std::int32_t)(game.isOver() ? 1 : 0);

        (void)client.send(state);
    }

    std::cout << "Game over!" << std::endl;
    return 0;
}
