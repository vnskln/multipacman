#include <SFML/Network.hpp>
#include <iostream>
#include "NetworkDefs.h"

int main() {
    sf::TcpListener listener;
    if (listener.listen(SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Cannot listen on port " << SERVER_PORT << std::endl;
        return 1;
    }
    std::cout << "Server started on port " << SERVER_PORT << std::endl;
    std::cout << "Waiting for connection..." << std::endl;

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Status::Done) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }
    std::cout << "Client connected!" << std::endl;

    sf::Packet packet;
    packet << std::string("Hello from server!");
    (void)client.send(packet);

    std::cout << "Message sent. Done." << std::endl;
    return 0;
}
