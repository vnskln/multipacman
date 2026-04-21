#include "LobbyScreen.h"
#include "Constants.h"
#include "Protocol.h"

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

        if (client.isHost() && mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 370 && my <= 420) {
            client.sendChangeMap();
        }

        if (client.isHost() && mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 430 && my <= 480) {
            client.sendChangeDifficulty();
        }

        if (client.isHost() && mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 490 && my <= 540) {
            client.sendAddBot();
        }

        if (client.isHost() && mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 550 && my <= 600) {
            client.sendStartGame();
        }

        if (mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 610 && my <= 660) {
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
    int hostId = -1;
    for (int i = 0; i < (int)players.size(); i++) {
        if (hostId < 0 || players[i].id < hostId) hostId = players[i].id;
    }
    for (int i = 0; i < (int)players.size(); i++) {
        std::string info = players[i].name;
        if (players[i].id == hostId) info += " (host)";

        sf::RectangleShape colorBox({20.f, 20.f});
        colorBox.setPosition({CENTER_X - 120.f, 180.f + i * 40.f});
        colorBox.setFillColor(getPlayerColor(players[i].id));
        window.draw(colorBox);

        sf::Text playerText(font, info, 22);
        playerText.setPosition({CENTER_X - 90.f, 178.f + i * 40.f});
        playerText.setFillColor(sf::Color::White);
        window.draw(playerText);
    }

    std::string diffName = "Normal";
    if (client.getLobbyDifficulty() == DIFFICULTY_EASY) diffName = "Easy";
    if (client.getLobbyDifficulty() == DIFFICULTY_HARD) diffName = "Hard";

    std::string mapInfo = "Map: " + client.getLobbyMapName() + "  |  Difficulty: " + diffName;
    sf::Text mapText(font, mapInfo, 20);
    sf::FloatRect mapBounds = mapText.getGlobalBounds();
    mapText.setPosition({CENTER_X - mapBounds.size.x / 2.f, 340.f});
    mapText.setFillColor(sf::Color(200, 200, 200));
    window.draw(mapText);

    if (client.isHost()) {
        sf::RectangleShape mapBtn({200.f, 50.f});
        mapBtn.setPosition({CENTER_X - 100.f, 370.f});
        mapBtn.setFillColor(sf::Color(100, 0, 150));
        window.draw(mapBtn);

        sf::Text mapBtnText(font, "Change Map", 22);
        sf::FloatRect mbBounds = mapBtnText.getGlobalBounds();
        mapBtnText.setPosition({CENTER_X - mbBounds.size.x / 2.f, 382.f});
        mapBtnText.setFillColor(sf::Color::White);
        window.draw(mapBtnText);

        sf::RectangleShape diffBtn({200.f, 50.f});
        diffBtn.setPosition({CENTER_X - 100.f, 430.f});
        diffBtn.setFillColor(sf::Color(150, 75, 0));
        window.draw(diffBtn);

        sf::Text diffBtnText(font, "Change Difficulty", 22);
        sf::FloatRect dbBounds = diffBtnText.getGlobalBounds();
        diffBtnText.setPosition({CENTER_X - dbBounds.size.x / 2.f, 442.f});
        diffBtnText.setFillColor(sf::Color::White);
        window.draw(diffBtnText);

        if ((int)client.getLobbyPlayers().size() < MAX_PLAYERS) {
            sf::RectangleShape botBtn({200.f, 50.f});
            botBtn.setPosition({CENTER_X - 100.f, 490.f});
            botBtn.setFillColor(sf::Color(0, 0, 200));
            window.draw(botBtn);

            sf::Text botText(font, "Add Bot", 22);
            sf::FloatRect bbBounds = botText.getGlobalBounds();
            botText.setPosition({CENTER_X - bbBounds.size.x / 2.f, 502.f});
            botText.setFillColor(sf::Color::White);
            window.draw(botText);
        }

        sf::RectangleShape startBtn({200.f, 50.f});
        startBtn.setPosition({CENTER_X - 100.f, 550.f});
        startBtn.setFillColor(sf::Color(0, 150, 0));
        window.draw(startBtn);

        sf::Text startText(font, "Start Game", 22);
        sf::FloatRect stBounds = startText.getGlobalBounds();
        startText.setPosition({CENTER_X - stBounds.size.x / 2.f, 562.f});
        startText.setFillColor(sf::Color::White);
        window.draw(startText);
    } else {
        sf::Text waitText(font, "Waiting for host...", 20);
        sf::FloatRect wtBounds = waitText.getGlobalBounds();
        waitText.setPosition({CENTER_X - wtBounds.size.x / 2.f, 562.f});
        waitText.setFillColor(sf::Color(150, 150, 150));
        window.draw(waitText);
    }

    sf::RectangleShape dcBtn({200.f, 50.f});
    dcBtn.setPosition({CENTER_X - 100.f, 610.f});
    dcBtn.setFillColor(sf::Color(150, 0, 0));
    window.draw(dcBtn);

    sf::Text dcText(font, "Disconnect", 22);
    sf::FloatRect dcBounds = dcText.getGlobalBounds();
    dcText.setPosition({CENTER_X - dcBounds.size.x / 2.f, 622.f});
    dcText.setFillColor(sf::Color::White);
    window.draw(dcText);
}

bool LobbyScreen::wantsDisconnect() const {
    return disconnectClicked;
}

void LobbyScreen::reset() {
    disconnectClicked = false;
}
