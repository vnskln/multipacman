#include "GameClient.h"
#include <iostream>

GameClient::GameClient()
    : myPlayerId(-1), connected(false), inLobby(false),
      gameStarted(false), gameOver(false) {
}

bool GameClient::connect(const std::string& ip, unsigned short port, const std::string& name) {
    game = Game();
    myPlayerId = -1;
    inLobby = false;
    gameStarted = false;
    gameOver = false;
    lobbyPlayers.clear();
    selector.clear();

    auto address = sf::IpAddress::resolve(ip);
    if (!address) {
        std::cerr << "Invalid IP" << std::endl;
        return false;
    }

    if (socket.connect(*address, port, sf::seconds(5)) != sf::Socket::Status::Done) {
        std::cerr << "Cannot connect" << std::endl;
        return false;
    }

    connected = true;
    inLobby = true;
    selector.add(socket);

    sf::Packet joinPacket;
    joinPacket << MSG_JOIN << name;
    (void)socket.send(joinPacket);

    return true;
}

void GameClient::sendInput(std::int32_t dir) {
    sf::Packet packet;
    packet << MSG_PLAYER_INPUT << dir;
    (void)socket.send(packet);
}

void GameClient::sendStartGame() {
    sf::Packet packet;
    packet << MSG_START_GAME;
    (void)socket.send(packet);
}

void GameClient::receiveMessages() {
    if (!connected) return;

    if (!selector.wait(sf::milliseconds(1))) return;

    if (!selector.isReady(socket)) return;

    sf::Packet packet;
    sf::Socket::Status status = socket.receive(packet);

    if (status == sf::Socket::Status::Disconnected) {
        std::cout << "Server disconnected" << std::endl;
        connected = false;
        return;
    }

    if (status != sf::Socket::Status::Done) return;

    std::int32_t msgType;
    packet >> msgType;

    if (msgType == MSG_JOIN_ACCEPTED) handleJoinAccepted(packet);
    if (msgType == MSG_JOIN_REJECTED) handleJoinRejected(packet);
    if (msgType == MSG_LOBBY_UPDATE) handleLobbyUpdate(packet);
    if (msgType == MSG_GAME_STARTED) handleGameStarted();
    if (msgType == MSG_GAME_STATE) handleGameState(packet);
    if (msgType == MSG_GAME_OVER) handleGameOver(packet);
}

void GameClient::handleJoinAccepted(sf::Packet& packet) {
    std::int32_t id;
    packet >> id;
    myPlayerId = id;
    std::cout << "Joined as player " << id << std::endl;
}

void GameClient::handleJoinRejected(sf::Packet& packet) {
    std::string reason;
    packet >> reason;
    std::cout << "Join rejected: " << reason << std::endl;
    connected = false;
}

void GameClient::handleLobbyUpdate(sf::Packet& packet) {
    std::int32_t count;
    packet >> count;

    lobbyPlayers.clear();
    for (int i = 0; i < count; i++) {
        std::int32_t id;
        std::string name;
        packet >> id >> name;
        LobbyPlayer lp;
        lp.id = id;
        lp.name = name;
        lobbyPlayers.push_back(lp);
    }
}

void GameClient::handleGameStarted() {
    inLobby = false;
    gameStarted = true;

    for (int i = 0; i < (int)lobbyPlayers.size(); i++) {
        game.addPlayer(lobbyPlayers[i].id, lobbyPlayers[i].name);
    }
    game.start();
}

void GameClient::handleGameState(sf::Packet& packet) {
    std::int32_t playerCount;
    packet >> playerCount;

    std::vector<Player>& players = game.getPlayers();
    for (int i = 0; i < playerCount; i++) {
        std::int32_t id, x, y, score, state;
        packet >> id >> x >> y >> score >> state;

        if (i < (int)players.size()) {
            players[i].setPosition(x, y);
            players[i].setScore(score);

            if (state == 0) players[i].setState(Player::State::Alive);
            if (state == 1) players[i].setState(Player::State::Dead);
            if (state == 2) players[i].setState(Player::State::Disconnected);
        }
    }

    std::int32_t ghostCount;
    packet >> ghostCount;

    std::vector<Ghost>& ghosts = game.getGhosts();
    for (int i = 0; i < ghostCount; i++) {
        std::int32_t id, x, y;
        packet >> id >> x >> y;

        if (i < (int)ghosts.size()) {
            ghosts[i].setPosition(x, y);
        }
    }

    std::int32_t dotCount;
    packet >> dotCount;

    std::vector<Dot>& dots = game.getDots();
    for (int i = 0; i < dotCount; i++) {
        std::int32_t x, y, collected;
        packet >> x >> y >> collected;

        if (i < (int)dots.size()) {
            dots[i].setCollected(collected == 1);
        }
    }

    std::int32_t over;
    packet >> over;
    if (over == 1) {
        gameOver = true;
    }
}

void GameClient::handleGameOver(sf::Packet& packet) {
    gameOver = true;

    std::int32_t count;
    packet >> count;

    std::cout << std::endl << "=== RANKING ===" << std::endl;
    for (int i = 0; i < count; i++) {
        std::int32_t id, score;
        std::string name;
        packet >> id >> name >> score;
        std::cout << name << ": " << score << " pts" << std::endl;
    }
}

void GameClient::disconnect() {
    socket.disconnect();
    connected = false;
}

Game& GameClient::getGame() {
    return game;
}

int GameClient::getMyPlayerId() const {
    return myPlayerId;
}

bool GameClient::isConnected() const {
    return connected;
}

bool GameClient::isInLobby() const {
    return inLobby;
}

bool GameClient::isGameStarted() const {
    return gameStarted;
}

bool GameClient::isGameOver() const {
    return gameOver;
}

bool GameClient::isHost() const {
    return myPlayerId == 0;
}

const std::vector<LobbyPlayer>& GameClient::getLobbyPlayers() const {
    return lobbyPlayers;
}
