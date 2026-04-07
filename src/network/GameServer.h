#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <SFML/Network.hpp>
#include <vector>
#include <memory>
#include "Game.h"
#include "ClientConnection.h"
#include "Protocol.h"

class GameServer {
private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<std::unique_ptr<ClientConnection>> clients;
    Game game;
    bool inGame;
    int nextPlayerId;

    void acceptNewClient();
    void handleLobbyMessage(ClientConnection& client);
    void handleGameMessage(ClientConnection& client);
    void removeClient(ClientConnection& client);
    Direction directionFromNetwork(std::int32_t dir);

    void broadcastLobbyUpdate();
    void broadcastGameState();
    void broadcastGameOver();
    void broadcast(sf::Packet& packet);

public:
    GameServer(unsigned short port);
    void run();
};

#endif
