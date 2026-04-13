#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <SFML/Network.hpp>
#include <string>
#include <memory>

/// Polaczenie z jednym klientem po stronie serwera.
class ClientConnection {
private:
    std::unique_ptr<sf::TcpSocket> socket;
    int playerId;
    std::string name;
    bool connected;

public:
    ClientConnection();

    sf::TcpSocket& getSocket();
    int getPlayerId() const;
    std::string getName() const;
    bool isConnected() const;

    void setPlayerId(int id);
    void setName(const std::string& n);
    void disconnect();

    /// Wysyla pakiet do tego klienta. Zwraca false jesli sie nie udalo.
    bool send(sf::Packet& packet);
};

#endif
