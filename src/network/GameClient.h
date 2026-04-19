#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include "Game.h"
#include "Protocol.h"

/// Dane gracza widoczne w lobby.
struct LobbyPlayer {
    int id;
    std::string name;
};

/**
 * Klient gry. Laczy sie z serwerem, wysyla input
 * i odbiera stan gry do wyswietlenia.
 */
class GameClient {
private:
    sf::TcpSocket socket;
    sf::SocketSelector selector;
    Game game;
    int myPlayerId;
    bool connected;
    bool inLobby;
    bool gameStarted;
    bool gameOver;

    std::vector<LobbyPlayer> lobbyPlayers;

    std::string lobbyMapName;

    void handleJoinAccepted(sf::Packet& packet);
    void handleJoinRejected(sf::Packet& packet);
    void handleLobbyUpdate(sf::Packet& packet);
    void handleGameStarted(sf::Packet& packet);
    void handleGameState(sf::Packet& packet);
    void handleGameOver(sf::Packet& packet);

public:
    GameClient();

    /// Laczy sie z serwerem i wysyla JOIN. Zwraca false jesli nie udalo sie polaczyc.
    bool connect(const std::string& ip, unsigned short port, const std::string& name);
    /// Wysyla kierunek ruchu do serwera.
    void sendInput(std::int32_t dir);
    void sendStartGame();
    /// Wysyla zadanie dodania bota do serwera (tylko host).
    void sendAddBot();
    /// Wysyla zadanie zmiany mapy do serwera (tylko host).
    void sendChangeMap();
    /// Odbiera i przetwarza wiadomosci z serwera. Nalezy wolac co klatke.
    void receiveMessages();
    void disconnect();

    Game& getGame();
    int getMyPlayerId() const;
    bool isConnected() const;
    bool isInLobby() const;
    bool isGameStarted() const;
    bool isGameOver() const;
    bool isHost() const;
    const std::vector<LobbyPlayer>& getLobbyPlayers() const;
    const std::string& getLobbyMapName() const;
};

#endif
