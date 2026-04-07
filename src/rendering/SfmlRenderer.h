#ifndef SFMLRENDERER_H
#define SFMLRENDERER_H

#include "Renderer.h"
#include <SFML/Graphics.hpp>

class SfmlRenderer : public Renderer {
private:
    sf::RenderWindow& window;
    sf::Font& font;

public:
    SfmlRenderer(sf::RenderWindow& window, sf::Font& font);
    void render(const Game& game) override;
};

#endif
