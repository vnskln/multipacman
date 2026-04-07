#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include "Screen.h"
#include "GameClient.h"

class LobbyScreen : public Screen {
private:
    sf::Font& font;
    GameClient& client;
    bool disconnectClicked;

public:
    LobbyScreen(sf::Font& font, GameClient& client);
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    bool wantsDisconnect() const;
    void reset();
};

#endif
