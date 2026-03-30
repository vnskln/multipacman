#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <SFML/Network.hpp>
#include <string>
#include <memory>

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

    bool send(sf::Packet& packet);
};

#endif
