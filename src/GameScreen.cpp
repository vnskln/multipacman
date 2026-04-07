#include "GameScreen.h"
#include "Protocol.h"
#include <string>
#include <vector>

GameScreen::GameScreen(sf::Font& font, sf::RenderWindow& window, GameClient& client)
    : font(font), client(client), renderer(window, font), backToMenu(false) {
}

void GameScreen::handleEvent(const sf::Event& event) {
    auto* key = event.getIf<sf::Event::KeyPressed>();
    if (key) {
        if (key->code == sf::Keyboard::Key::Escape) {
            backToMenu = true;
            return;
        }

        std::int32_t dir = DIR_NONE;
        if (key->code == sf::Keyboard::Key::W || key->code == sf::Keyboard::Key::Up)
            dir = DIR_UP;
        if (key->code == sf::Keyboard::Key::S || key->code == sf::Keyboard::Key::Down)
            dir = DIR_DOWN;
        if (key->code == sf::Keyboard::Key::A || key->code == sf::Keyboard::Key::Left)
            dir = DIR_LEFT;
        if (key->code == sf::Keyboard::Key::D || key->code == sf::Keyboard::Key::Right)
            dir = DIR_RIGHT;

        if (dir != DIR_NONE) {
            client.sendInput(dir);
        }
    }

    if (client.isGameOver()) {
        auto* click = event.getIf<sf::Event::MouseButtonPressed>();
        if (click) {
            float mx = (float)click->position.x;
            float my = (float)click->position.y;
            if (mx >= 300 && mx <= 500 && my >= 450 && my <= 500) {
                backToMenu = true;
            }
        }
    }
}

void GameScreen::update() {
}

void GameScreen::draw(sf::RenderWindow& window) {
    if (client.isGameStarted()) {
        renderer.render(client.getGame());
    }

    Game& game = client.getGame();
    const std::vector<Player>& players = game.getPlayers();
    int myId = client.getMyPlayerId();

    bool amDead = false;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getPlayerId() == myId && !players[i].isAlive()) {
            amDead = true;
            break;
        }
    }

    if (amDead && !client.isGameOver()) {
        sf::Text diedText(font, "You died!", 36);
        sf::FloatRect bounds = diedText.getGlobalBounds();
        diedText.setPosition({400.f - bounds.size.x / 2.f, 420.f});
        diedText.setFillColor(sf::Color::Red);
        window.draw(diedText);
    }

    if (client.isGameOver()) {
        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        sf::Text gameOverText(font, "GAME OVER", 48);
        sf::FloatRect goBounds = gameOverText.getGlobalBounds();
        gameOverText.setPosition({400.f - goBounds.size.x / 2.f, 150.f});
        gameOverText.setFillColor(sf::Color::White);
        window.draw(gameOverText);

        std::vector<const Player*> sorted;
        for (int i = 0; i < (int)players.size(); i++) {
            sorted.push_back(&players[i]);
        }
        for (int i = 0; i < (int)sorted.size() - 1; i++) {
            for (int j = i + 1; j < (int)sorted.size(); j++) {
                if (sorted[j]->getScore() > sorted[i]->getScore()) {
                    const Player* temp = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = temp;
                }
            }
        }

        for (int i = 0; i < (int)sorted.size(); i++) {
            std::string info = std::to_string(i + 1) + ". " + sorted[i]->getName()
                             + ": " + std::to_string(sorted[i]->getScore()) + " pts";
            sf::Text rankText(font, info, 22);
            sf::FloatRect rBounds = rankText.getGlobalBounds();
            rankText.setPosition({400.f - rBounds.size.x / 2.f, 250.f + i * 35.f});
            rankText.setFillColor(sf::Color::White);
            window.draw(rankText);
        }

        sf::RectangleShape menuBtn({200.f, 50.f});
        menuBtn.setPosition({300.f, 450.f});
        menuBtn.setFillColor(sf::Color(0, 100, 200));
        window.draw(menuBtn);

        sf::Text menuText(font, "Back to Menu", 20);
        sf::FloatRect mtBounds = menuText.getGlobalBounds();
        menuText.setPosition({400.f - mtBounds.size.x / 2.f, 462.f});
        menuText.setFillColor(sf::Color::White);
        window.draw(menuText);
    }
}

bool GameScreen::wantsMenu() const {
    return backToMenu;
}

void GameScreen::reset() {
    backToMenu = false;
}
