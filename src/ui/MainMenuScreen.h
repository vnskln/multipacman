#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "Screen.h"

/// Ekran menu glownego z przyciskami Connect i Exit.
class MainMenuScreen : public Screen {
private:
    sf::Font& font;
    bool connectClicked;
    bool exitClicked;

public:
    MainMenuScreen(sf::Font& font);
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    bool wantsConnect() const;
    bool wantsExit() const;
    void reset();
};

#endif
