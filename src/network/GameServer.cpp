#include "GameServer.h"
#include <iostream>

GameServer::GameServer(unsigned short port) : inGame(false), nextPlayerId(0) {
    if (listener.listen(port) != sf::Socket::Status::Done) {
        std::cerr << "Cannot listen on port " << port << std::endl;
        return;
    }
    selector.add(listener);
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Waiting for players..." << std::endl;
}

void GameServer::run() {
    while (true) {
        if (!inGame) {
            if (selector.wait(sf::milliseconds(100))) {
                if (selector.isReady(listener)) {
                    acceptNewClient();
                }

                for (int i = 0; i < (int)clients.size(); i++) {
                    if (selector.isReady(clients[i]->getSocket())) {
                        handleLobbyMessage(*clients[i]);
                    }
                }
            }
        }

        if (inGame) {
            if (selector.wait(sf::milliseconds(200))) {
                for (int i = 0; i < (int)clients.size(); i++) {
                    if (clients[i]->isConnected() && selector.isReady(clients[i]->getSocket())) {
                        handleGameMessage(*clients[i]);
                    }
                }
            }

            game.update();
            broadcastGameState();

            if (game.isOver()) {
                broadcastGameOver();
                std::cout << "Game over!" << std::endl;

                for (int i = 0; i < (int)clients.size(); i++) {
                    selector.remove(clients[i]->getSocket());
                    clients[i]->disconnect();
                }
                clients.clear();

                game = Game();
                inGame = false;
                nextPlayerId = 0;

                std::cout << "Waiting for players..." << std::endl;
            }
        }
    }
}

void GameServer::acceptNewClient() {
    auto connection = std::make_unique<ClientConnection>();

    if (listener.accept(connection->getSocket()) != sf::Socket::Status::Done) {
        return;
    }

    if ((int)clients.size() >= MAX_PLAYERS) {
        sf::Packet reject;
        reject << MSG_JOIN_REJECTED << std::string("Server is full");
        (void)connection->getSocket().send(reject);
        connection->getSocket().disconnect();
        return;
    }

    selector.add(connection->getSocket());
    clients.push_back(std::move(connection));
    std::cout << "New connection, waiting for JOIN..." << std::endl;
}

void GameServer::handleLobbyMessage(ClientConnection& client) {
    sf::Packet packet;
    sf::Socket::Status status = client.getSocket().receive(packet);

    if (status == sf::Socket::Status::Disconnected) {
        removeClient(client);
        return;
    }

    if (status != sf::Socket::Status::Done) {
        return;
    }

    std::int32_t msgType;
    packet >> msgType;

    if (msgType == MSG_JOIN) {
        std::string name;
        packet >> name;

        int id = nextPlayerId;
        nextPlayerId++;

        client.setPlayerId(id);
        client.setName(name);
        game.addPlayer(id, name);

        sf::Packet accept;
        accept << MSG_JOIN_ACCEPTED << (std::int32_t)id;
        client.send(accept);

        std::cout << name << " joined as player " << id << std::endl;

        broadcastLobbyUpdate();
    }

    if (msgType == MSG_START_GAME) {
        if (client.getPlayerId() == 0) {
            game.start();
            inGame = true;

            sf::Packet started;
            started << MSG_GAME_STARTED;
            broadcast(started);

            std::cout << "Game started!" << std::endl;
        }
    }
}

void GameServer::handleGameMessage(ClientConnection& client) {
    sf::Packet packet;
    sf::Socket::Status status = client.getSocket().receive(packet);

    if (status == sf::Socket::Status::Disconnected) {
        removeClient(client);
        return;
    }

    if (status != sf::Socket::Status::Done) {
        return;
    }

    std::int32_t msgType;
    packet >> msgType;

    if (msgType == MSG_PLAYER_INPUT) {
        std::int32_t dir;
        packet >> dir;
        game.handleInput(client.getPlayerId(), directionFromNetwork(dir));
    }
}

void GameServer::removeClient(ClientConnection& client) {
    std::cout << client.getName() << " disconnected" << std::endl;

    selector.remove(client.getSocket());
    game.removePlayer(client.getPlayerId());
    client.disconnect();

    if (!inGame) {
        broadcastLobbyUpdate();
    }
}

Direction GameServer::directionFromNetwork(std::int32_t dir) {
    if (dir == DIR_UP) return Direction::Up;
    if (dir == DIR_DOWN) return Direction::Down;
    if (dir == DIR_LEFT) return Direction::Left;
    if (dir == DIR_RIGHT) return Direction::Right;
    return Direction::None;
}

void GameServer::broadcastLobbyUpdate() {
    sf::Packet packet;
    packet << MSG_LOBBY_UPDATE;

    std::int32_t count = 0;
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->getPlayerId() >= 0) {
            count++;
        }
    }
    packet << count;

    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->getPlayerId() >= 0) {
            packet << (std::int32_t)clients[i]->getPlayerId()
                   << clients[i]->getName();
        }
    }

    broadcast(packet);
}

void GameServer::broadcastGameState() {
    const std::vector<Player>& players = game.getPlayers();
    const std::vector<Ghost>& ghosts = game.getGhosts();
    const std::vector<Dot>& dots = game.getDots();

    sf::Packet packet;
    packet << MSG_GAME_STATE;

    packet << (std::int32_t)players.size();
    for (int i = 0; i < (int)players.size(); i++) {
        packet << (std::int32_t)players[i].getPlayerId()
               << (std::int32_t)players[i].getX()
               << (std::int32_t)players[i].getY()
               << (std::int32_t)players[i].getScore()
               << (std::int32_t)players[i].getState();
    }

    packet << (std::int32_t)ghosts.size();
    for (int i = 0; i < (int)ghosts.size(); i++) {
        packet << (std::int32_t)ghosts[i].getId()
               << (std::int32_t)ghosts[i].getX()
               << (std::int32_t)ghosts[i].getY();
    }

    packet << (std::int32_t)dots.size();
    for (int i = 0; i < (int)dots.size(); i++) {
        packet << (std::int32_t)dots[i].getX()
               << (std::int32_t)dots[i].getY()
               << (std::int32_t)(dots[i].isCollected() ? 1 : 0);
    }

    packet << (std::int32_t)(game.isOver() ? 1 : 0);

    broadcast(packet);
}

void GameServer::broadcastGameOver() {
    const std::vector<Player>& players = game.getPlayers();

    sf::Packet packet;
    packet << MSG_GAME_OVER;
    packet << (std::int32_t)players.size();
    for (int i = 0; i < (int)players.size(); i++) {
        packet << (std::int32_t)players[i].getPlayerId()
               << players[i].getName()
               << (std::int32_t)players[i].getScore();
    }

    broadcast(packet);
}

void GameServer::broadcast(sf::Packet& packet) {
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->isConnected()) {
            clients[i]->send(packet);
        }
    }
}
