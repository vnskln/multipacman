#include "ClientConnection.h"

ClientConnection::ClientConnection()
    : socket(new sf::TcpSocket()), playerId(-1), connected(false) {
}

sf::TcpSocket& ClientConnection::getSocket() {
    return *socket;
}

int ClientConnection::getPlayerId() const {
    return playerId;
}

std::string ClientConnection::getName() const {
    return name;
}

bool ClientConnection::isConnected() const {
    return connected;
}

void ClientConnection::setPlayerId(int id) {
    playerId = id;
    connected = true;
}

void ClientConnection::setName(const std::string& n) {
    name = n;
}

void ClientConnection::disconnect() {
    connected = false;
    socket->disconnect();
}

bool ClientConnection::send(sf::Packet& packet) {
    if (!connected) {
        return false;
    }
    return socket->send(packet) == sf::Socket::Status::Done;
}
