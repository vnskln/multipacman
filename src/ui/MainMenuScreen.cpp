#include "MainMenuScreen.h"
#include "Constants.h"

MainMenuScreen::MainMenuScreen(sf::Font& font)
    : font(font), connectClicked(false), exitClicked(false) {
}

void MainMenuScreen::handleEvent(const sf::Event& event) {
    auto* click = event.getIf<sf::Event::MouseButtonPressed>();
    if (click) {
        float mx = (float)click->position.x;
        float my = (float)click->position.y;

        if (mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 300 && my <= 350) {
            connectClicked = true;
        }
        if (mx >= CENTER_X - 100 && mx <= CENTER_X + 100 && my >= 380 && my <= 430) {
            exitClicked = true;
        }
    }
}

void MainMenuScreen::update() {
}

void MainMenuScreen::draw(sf::RenderWindow& window) {
    sf::Text title(font, "MULTIPACMAN", 48);
    sf::FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition({CENTER_X - titleBounds.size.x / 2.f, 120.f});
    title.setFillColor(sf::Color::Yellow);
    window.draw(title);

    sf::RectangleShape connectBtn({200.f, 50.f});
    connectBtn.setPosition({CENTER_X - 100.f, 300.f});
    connectBtn.setFillColor(sf::Color(0, 100, 200));
    window.draw(connectBtn);

    sf::Text connectText(font, "Connect", 24);
    sf::FloatRect ctBounds = connectText.getGlobalBounds();
    connectText.setPosition({CENTER_X - ctBounds.size.x / 2.f, 312.f});
    connectText.setFillColor(sf::Color::White);
    window.draw(connectText);

    sf::RectangleShape exitBtn({200.f, 50.f});
    exitBtn.setPosition({CENTER_X - 100.f, 380.f});
    exitBtn.setFillColor(sf::Color(0, 100, 200));
    window.draw(exitBtn);

    sf::Text exitText(font, "Exit", 24);
    sf::FloatRect etBounds = exitText.getGlobalBounds();
    exitText.setPosition({CENTER_X - etBounds.size.x / 2.f, 392.f});
    exitText.setFillColor(sf::Color::White);
    window.draw(exitText);
}

bool MainMenuScreen::wantsConnect() const {
    return connectClicked;
}

bool MainMenuScreen::wantsExit() const {
    return exitClicked;
}

void MainMenuScreen::reset() {
    connectClicked = false;
    exitClicked = false;
}
