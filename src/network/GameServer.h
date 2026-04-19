#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <SFML/Network.hpp>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "Game.h"
#include "ClientConnection.h"
#include "Protocol.h"

/**
 * Serwer gry. Obsluguje lobby i rozgrywke dla 1-4 graczy.
 * Dziala w petli: odbiera input, aktualizuje stan, rozsyla do klientow.
 */
class GameServer {
private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<std::unique_ptr<ClientConnection>> clients;
    Game game;
    bool inGame;
    int nextPlayerId;

    std::vector<std::pair<int, std::string>> pendingBots;
    std::vector<std::string> availableMaps;
    int selectedMapIndex;
    std::string currentMapName;

    sf::Clock gameClock;

    void scanMaps();
    void acceptNewClient();
    void handleLobbyMessage(ClientConnection& client);
    void handleGameMessage(ClientConnection& client);
    void removeClient(ClientConnection& client);
    Direction directionFromNetwork(std::int32_t dir);
    int countLobbyPlayers() const;

    void broadcastLobbyUpdate();
    void broadcastGameState();
    void broadcastGameOver();
    void broadcast(sf::Packet& packet);

public:
    /// @param port port TCP na ktorym serwer nasluchuje
    GameServer(unsigned short port);
    /// Glowna petla serwera (lobby + gra). Nie wraca.
    void run();
};

#endif
