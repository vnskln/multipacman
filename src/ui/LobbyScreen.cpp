#include "LobbyScreen.h"
#include "Constants.h"

static sf::Color getPlayerColor(int id) {
    if (id == 0) return sf::Color::Yellow;
    if (id == 1) return sf::Color::Green;
    if (id == 2) return sf::Color(0, 100, 255);
    if (id == 3) return sf::Color(255, 165, 0);
    return sf::Color::White;
}

LobbyScreen::LobbyScreen(sf::Font& font, GameClient& client)
    : font(font), client(client), disconnectClicked(false) {
}

void LobbyScreen::handleEvent(const sf::Event& event) {
    auto* click = event.getIf<sf::Event::MouseButtonPressed>();
    if (click) {
        float mx = (float)click->position.x;
        float my = (float)click->position.y;

        if (client.isHost() && mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 400 && my <= 450) {
            client.sendStartGame();
        }

        if (mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 470 && my <= 520) {
            disconnectClicked = true;
        }
    }
}

void LobbyScreen::update() {
}

void LobbyScreen::draw(sf::RenderWindow& window) {
    sf::Text title(font, "Lobby", 36);
    sf::FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition({CENTER_X - titleBounds.size.x / 2.f, 60.f});
    title.setFillColor(sf::Color::White);
    window.draw(title);

    const std::vector<LobbyPlayer>& players = client.getLobbyPlayers();
    for (int i = 0; i < (int)players.size(); i++) {
        std::string info = players[i].name;
        if (players[i].id == 0) info += " (host)";

        sf::RectangleShape colorBox({20.f, 20.f});
        colorBox.setPosition({CENTER_X - 120.f, 180.f + i * 40.f});
        colorBox.setFillColor(getPlayerColor(players[i].id));
        window.draw(colorBox);

        sf::Text playerText(font, info, 22);
        playerText.setPosition({CENTER_X - 90.f, 178.f + i * 40.f});
        playerText.setFillColor(sf::Color::White);
        window.draw(playerText);
    }

    if (client.isHost()) {
        sf::RectangleShape startBtn({200.f, 50.f});
        startBtn.setPosition({CENTER_X - 100.f, 400.f});
        startBtn.setFillColor(sf::Color(0, 150, 0));
        window.draw(startBtn);

        sf::Text startText(font, "Start Game", 22);
        sf::FloatRect stBounds = startText.getGlobalBounds();
        startText.setPosition({CENTER_X - stBounds.size.x / 2.f, 412.f});
        startText.setFillColor(sf::Color::White);
        window.draw(startText);
    } else {
        sf::Text waitText(font, "Waiting for host...", 20);
        sf::FloatRect wtBounds = waitText.getGlobalBounds();
        waitText.setPosition({CENTER_X - wtBounds.size.x / 2.f, 412.f});
        waitText.setFillColor(sf::Color(150, 150, 150));
        window.draw(waitText);
    }

    sf::RectangleShape dcBtn({200.f, 50.f});
    dcBtn.setPosition({CENTER_X - 100.f, 470.f});
    dcBtn.setFillColor(sf::Color(150, 0, 0));
    window.draw(dcBtn);

    sf::Text dcText(font, "Disconnect", 22);
    sf::FloatRect dcBounds = dcText.getGlobalBounds();
    dcText.setPosition({CENTER_X - dcBounds.size.x / 2.f, 482.f});
    dcText.setFillColor(sf::Color::White);
    window.draw(dcText);
}

bool LobbyScreen::wantsDisconnect() const {
    return disconnectClicked;
}

void LobbyScreen::reset() {
    disconnectClicked = false;
}
