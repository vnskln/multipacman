#include "GameServer.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

GameServer::GameServer(unsigned short port)
    : inGame(false), nextPlayerId(0), selectedMapIndex(0),
      currentDifficulty(DIFFICULTY_NORMAL) {

    scanMaps();
    currentMapName = availableMaps[0];

    if (listener.listen(port) != sf::Socket::Status::Done) {
        std::cerr << "Cannot listen on port " << port << std::endl;
        return;
    }
    selector.add(listener);
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Maps available: " << (int)availableMaps.size() << std::endl;
    std::cout << "Waiting for players..." << std::endl;
}

void GameServer::scanMaps() {
    availableMaps.clear();
    if (fs::exists("maps") && fs::is_directory("maps")) {
        for (auto& entry : fs::directory_iterator("maps")) {
            if (entry.path().extension() == ".map") {
                availableMaps.push_back(entry.path().stem().string());
            }
        }
    }
    std::sort(availableMaps.begin(), availableMaps.end());
    if (availableMaps.empty()) {
        availableMaps.push_back("default");
    }
}

void GameServer::run() {
    while (true) {
        cleanupDisconnectedClients();

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
            if (selector.wait(sf::milliseconds(10))) {
                if (selector.isReady(listener)) {
                    acceptNewClient();
                }
                for (int i = 0; i < (int)clients.size(); i++) {
                    if (clients[i]->isConnected() && selector.isReady(clients[i]->getSocket())) {
                        handleGameMessage(*clients[i]);
                    }
                }
            }

            if (gameClock.getElapsedTime() >= sf::milliseconds(100)) {
                gameClock.restart();
                game.update();
                broadcastGameState();
            }

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
                pendingBots.clear();

                std::cout << "Waiting for players..." << std::endl;
            }
        }
    }
}

int GameServer::countLobbyPlayers() const {
    int count = 0;
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->isConnected() && clients[i]->getPlayerId() >= 0) count++;
    }
    count += (int)pendingBots.size();
    return count;
}

void GameServer::cleanupDisconnectedClients() {
    for (int i = (int)clients.size() - 1; i >= 0; i--) {
        if (clients[i]->getPlayerId() >= 0 && !clients[i]->isConnected()) {
            clients.erase(clients.begin() + i);
        }
    }
}

bool GameServer::isHostClient(const ClientConnection& client) const {
    if (!client.isConnected()) return false;
    int clientId = client.getPlayerId();
    if (clientId < 0) return false;
    for (int i = 0; i < (int)clients.size(); i++) {
        if (!clients[i]->isConnected()) continue;
        int otherId = clients[i]->getPlayerId();
        if (otherId >= 0 && otherId < clientId) return false;
    }
    return true;
}

void GameServer::acceptNewClient() {
    auto connection = std::make_unique<ClientConnection>();

    if (listener.accept(connection->getSocket()) != sf::Socket::Status::Done) {
        return;
    }

    if (inGame) {
        sf::Packet reject;
        reject << MSG_JOIN_REJECTED << std::string("Game in progress");
        (void)connection->getSocket().send(reject);
        connection->getSocket().disconnect();
        return;
    }

    if (countLobbyPlayers() >= MAX_PLAYERS) {
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

        sf::Packet accept;
        accept << MSG_JOIN_ACCEPTED << (std::int32_t)id;
        client.send(accept);

        std::cout << name << " joined as player " << id << std::endl;

        broadcastLobbyUpdate();
    }

    if (msgType == MSG_ADD_BOT) {
        if (isHostClient(client)) {
            if (countLobbyPlayers() >= MAX_PLAYERS) return;

            int botId = nextPlayerId;
            nextPlayerId++;

            std::string botName = "Bot " + std::to_string((int)pendingBots.size() + 1);
            pendingBots.push_back({botId, botName});

            std::cout << botName << " added as player " << botId << std::endl;

            broadcastLobbyUpdate();
        }
    }

    if (msgType == MSG_CHANGE_MAP) {
        if (isHostClient(client) && (int)availableMaps.size() > 1) {
            selectedMapIndex = (selectedMapIndex + 1) % (int)availableMaps.size();
            currentMapName = availableMaps[selectedMapIndex];
            std::cout << "Map changed to: " << currentMapName << std::endl;
            broadcastLobbyUpdate();
        }
    }

    if (msgType == MSG_CHANGE_DIFFICULTY) {
        if (isHostClient(client)) {
            currentDifficulty = (currentDifficulty + 1) % 3;
            std::cout << "Difficulty changed to: " << currentDifficulty << std::endl;
            broadcastLobbyUpdate();
        }
    }

    if (msgType == MSG_START_GAME) {
        if (isHostClient(client) && countLobbyPlayers() > 0) {
            std::string mapPath = "maps/" + currentMapName + ".map";
            game = Game(mapPath);

            int divisor = 2;
            if (currentDifficulty == DIFFICULTY_EASY) divisor = 3;
            if (currentDifficulty == DIFFICULTY_HARD) divisor = 1;
            game.setGhostTickDivisor(divisor);

            for (int i = 0; i < (int)clients.size(); i++) {
                if (clients[i]->isConnected() && clients[i]->getPlayerId() >= 0) {
                    game.addPlayer(clients[i]->getPlayerId(), clients[i]->getName());
                }
            }
            for (int i = 0; i < (int)pendingBots.size(); i++) {
                game.addBot(pendingBots[i].first, pendingBots[i].second);
            }

            game.start();
            inGame = true;
            gameClock.restart();

            sf::Packet started;
            started << MSG_GAME_STARTED << currentMapName;

            const std::vector<std::string>& lines = game.getMap().getLayoutLines();
            started << (std::int32_t)lines.size();
            for (int i = 0; i < (int)lines.size(); i++) {
                started << lines[i];
            }

            broadcast(started);

            std::cout << "Game started on map: " << currentMapName << std::endl;
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
    if (inGame) {
        game.removePlayer(client.getPlayerId());
    }
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
    packet << currentMapName;
    packet << currentDifficulty;

    std::int32_t count = (std::int32_t)countLobbyPlayers();
    packet << count;

    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->isConnected() && clients[i]->getPlayerId() >= 0) {
            packet << (std::int32_t)clients[i]->getPlayerId()
                   << clients[i]->getName();
        }
    }
    for (int i = 0; i < (int)pendingBots.size(); i++) {
        packet << (std::int32_t)pendingBots[i].first
               << pendingBots[i].second;
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
               << (std::int32_t)ghosts[i].getY()
               << (std::int32_t)(ghosts[i].isFrightened() ? 1 : 0)
               << (std::int32_t)(ghosts[i].isRespawning() ? 1 : 0);
    }

    packet << (std::int32_t)dots.size();
    for (int i = 0; i < (int)dots.size(); i++) {
        packet << (std::int32_t)dots[i].getX()
               << (std::int32_t)dots[i].getY()
               << (std::int32_t)(dots[i].isCollected() ? 1 : 0)
               << (std::int32_t)(dots[i].isPowerPellet() ? 1 : 0);
    }

    packet << (std::int32_t)(game.isOver() ? 1 : 0);

    broadcast(packet);
}

void GameServer::broadcastGameOver() {
    const std::vector<Player>& players = game.getPlayers();

    sf::Packet packet;
    packet << MSG_GAME_OVER;
    packet << (std::int32_t)(game.isWon() ? 1 : 0);
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
