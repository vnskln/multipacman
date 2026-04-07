#ifndef CONNECTSCREEN_H
#define CONNECTSCREEN_H

#include "Screen.h"
#include <string>

class ConnectScreen : public Screen {
private:
    sf::Font& font;
    std::string ipText;
    std::string portText;
    std::string nameText;
    int activeField;
    bool connectClicked;
    bool backClicked;
    std::string errorMessage;

public:
    ConnectScreen(sf::Font& font);
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    std::string getIp() const;
    unsigned short getPort() const;
    std::string getName() const;
    bool wantsConnect() const;
    bool wantsBack() const;
    void setError(const std::string& msg);
    void clearConnect();
    void reset();
};

#endif
