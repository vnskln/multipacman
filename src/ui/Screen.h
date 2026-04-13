#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>

/// Klasa bazowa dla ekranow GUI (menu, lobby, gra itp.).
class Screen {
public:
    Screen() {}
    virtual ~Screen() {}
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif
