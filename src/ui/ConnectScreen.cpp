#include "ConnectScreen.h"
#include "Constants.h"
#include <cstdlib>

ConnectScreen::ConnectScreen(sf::Font& font)
    : font(font), ipText("127.0.0.1"), portText("53000"), nameText(""),
      activeField(2), connectClicked(false), backClicked(false) {
}

void ConnectScreen::handleEvent(const sf::Event& event) {
    auto* click = event.getIf<sf::Event::MouseButtonPressed>();
    if (click) {
        float mx = (float)click->position.x;
        float my = (float)click->position.y;

        if (mx >= CENTER_X - 50 && mx <= CENTER_X + 200) {
            if (my >= 200 && my <= 235) activeField = 0;
            if (my >= 260 && my <= 295) activeField = 1;
            if (my >= 320 && my <= 355) activeField = 2;
        }

        if (mx >= CENTER_X - 150 && mx <= CENTER_X - 10 && my >= 420 && my <= 465) {
            connectClicked = true;
        }
        if (mx >= CENTER_X + 20 && mx <= CENTER_X + 160 && my >= 420 && my <= 465) {
            backClicked = true;
        }
    }

    auto* key = event.getIf<sf::Event::KeyPressed>();
    if (key) {
        if (key->code == sf::Keyboard::Key::Tab) {
            activeField = (activeField + 1) % 3;
        }
        if (key->code == sf::Keyboard::Key::Backspace) {
            if (activeField == 0 && !ipText.empty()) ipText.pop_back();
            if (activeField == 1 && !portText.empty()) portText.pop_back();
            if (activeField == 2 && !nameText.empty()) nameText.pop_back();
        }
        if (key->code == sf::Keyboard::Key::Enter) {
            connectClicked = true;
        }
    }

    auto* text = event.getIf<sf::Event::TextEntered>();
    if (text) {
        if (text->unicode >= 32 && text->unicode < 127) {
            char c = (char)text->unicode;
            if (activeField == 0) ipText += c;
            if (activeField == 1) portText += c;
            if (activeField == 2 && nameText.size() < 16) nameText += c;
        }
    }
}

void ConnectScreen::update() {
}

void ConnectScreen::draw(sf::RenderWindow& window) {
    sf::Text title(font, "Connect to Server", 32);
    sf::FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition({CENTER_X - titleBounds.size.x / 2.f, 80.f});
    title.setFillColor(sf::Color::White);
    window.draw(title);

    std::string labels[3] = {"Server IP:", "Port:", "Player name:"};
    std::string* fields[3] = {&ipText, &portText, &nameText};
    float fieldY[3] = {200.f, 260.f, 320.f};

    for (int i = 0; i < 3; i++) {
        sf::Text label(font, labels[i], 20);
        label.setPosition({CENTER_X - 220.f, fieldY[i] + 5.f});
        label.setFillColor(sf::Color::White);
        window.draw(label);

        sf::RectangleShape field({250.f, 35.f});
        field.setPosition({CENTER_X - 50.f, fieldY[i]});
        field.setFillColor(sf::Color(30, 30, 30));
        if (i == activeField) {
            field.setOutlineColor(sf::Color::Yellow);
            field.setOutlineThickness(2.f);
        } else {
            field.setOutlineColor(sf::Color(100, 100, 100));
            field.setOutlineThickness(1.f);
        }
        window.draw(field);

        std::string display = *fields[i];
        if (i == activeField) display += "|";
        sf::Text fieldText(font, display, 18);
        fieldText.setPosition({CENTER_X - 45.f, fieldY[i] + 7.f});
        fieldText.setFillColor(sf::Color::White);
        window.draw(fieldText);
    }

    sf::RectangleShape connectBtn({140.f, 45.f});
    connectBtn.setPosition({CENTER_X - 150.f, 420.f});
    connectBtn.setFillColor(sf::Color(0, 100, 200));
    window.draw(connectBtn);

    sf::Text connectText(font, "Connect", 20);
    sf::FloatRect ctBounds = connectText.getGlobalBounds();
    connectText.setPosition({CENTER_X - 80.f - ctBounds.size.x / 2.f, 430.f});
    connectText.setFillColor(sf::Color::White);
    window.draw(connectText);

    sf::RectangleShape backBtn({140.f, 45.f});
    backBtn.setPosition({CENTER_X + 20.f, 420.f});
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);

    sf::Text backText(font, "Back", 20);
    sf::FloatRect btBounds = backText.getGlobalBounds();
    backText.setPosition({CENTER_X + 90.f - btBounds.size.x / 2.f, 430.f});
    backText.setFillColor(sf::Color::White);
    window.draw(backText);

    if (!errorMessage.empty()) {
        sf::Text error(font, errorMessage, 18);
        sf::FloatRect errorBounds = error.getGlobalBounds();
        error.setPosition({CENTER_X - errorBounds.size.x / 2.f, 490.f});
        error.setFillColor(sf::Color::Red);
        window.draw(error);
    }
}

std::string ConnectScreen::getIp() const {
    return ipText;
}

unsigned short ConnectScreen::getPort() const {
    int port = std::atoi(portText.c_str());
    if (port <= 0 || port > 65535) return 53000;
    return (unsigned short)port;
}

std::string ConnectScreen::getName() const {
    return nameText;
}

bool ConnectScreen::wantsConnect() const {
    return connectClicked;
}

bool ConnectScreen::wantsBack() const {
    return backClicked;
}

void ConnectScreen::setError(const std::string& msg) {
    errorMessage = msg;
}

void ConnectScreen::clearConnect() {
    connectClicked = false;
}

void ConnectScreen::reset() {
    ipText = "127.0.0.1";
    portText = "53000";
    nameText = "";
    activeField = 2;
    connectClicked = false;
    backClicked = false;
    errorMessage = "";
}
