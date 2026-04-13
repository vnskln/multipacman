#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Screen.h"
#include "SfmlRenderer.h"
#include "GameClient.h"

/// Ekran rozgrywki - rysuje plansze, obsluguje input i ekran game over.
class GameScreen : public Screen {
private:
    sf::Font& font;
    GameClient& client;
    SfmlRenderer renderer;
    bool backToMenu;

public:
    GameScreen(sf::Font& font, sf::RenderWindow& window, GameClient& client);
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    bool wantsMenu() const;
    void reset();
};

#endif
