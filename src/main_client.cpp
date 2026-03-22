#include <SFML/Network.hpp>
#include <iostream>
#include <string>
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
        std::cerr << "Cannot connect to server" << std::endl;
        return 1;
    }
    std::cout << "Connected!" << std::endl;

    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Status::Done) {
        std::string message;
        packet >> message;
        std::cout << "Server says: " << message << std::endl;
    }

    socket.disconnect();
    return 0;
}
