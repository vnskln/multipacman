#include "SfmlRenderer.h"
#include "Constants.h"

static const int MAP_X = MAP_MARGIN;
static const int MAP_Y = MAP_MARGIN;

static sf::Color getPlayerColor(int id) {
    if (id == 0) return sf::Color::Yellow;
    if (id == 1) return sf::Color::Green;
    if (id == 2) return sf::Color(0, 100, 255);
    if (id == 3) return sf::Color(255, 165, 0);
    return sf::Color::White;
}

static sf::Color getGhostColor(int id) {
    if (id == 0) return sf::Color::Red;
    if (id == 1) return sf::Color(255, 105, 180);
    if (id == 2) return sf::Color::Cyan;
    if (id == 3) return sf::Color(148, 0, 211);
    return sf::Color::Red;
}

SfmlRenderer::SfmlRenderer(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {
}

void SfmlRenderer::render(const Game& game) {
    const Map& map = game.getMap();
    const std::vector<Player>& players = game.getPlayers();
    const std::vector<Ghost>& ghosts = game.getGhosts();
    const std::vector<Dot>& dots = game.getDots();

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.isWall(x, y)) {
                sf::RectangleShape wall({(float)TILE, (float)TILE});
                wall.setPosition({(float)(MAP_X + x * TILE), (float)(MAP_Y + y * TILE)});
                wall.setFillColor(sf::Color(0, 0, 139));
                window.draw(wall);
            }
        }
    }

    for (int i = 0; i < (int)dots.size(); i++) {
        if (dots[i].isCollected()) continue;
        float cx = MAP_X + dots[i].getX() * TILE + TILE / 2.f - 3.f;
        float cy = MAP_Y + dots[i].getY() * TILE + TILE / 2.f - 3.f;
        sf::CircleShape dot(3.f);
        dot.setPosition({cx, cy});
        dot.setFillColor(sf::Color::White);
        window.draw(dot);
    }

    for (int i = 0; i < (int)ghosts.size(); i++) {
        sf::RectangleShape ghost({(float)TILE - 4, (float)TILE - 4});
        ghost.setPosition({(float)(MAP_X + ghosts[i].getX() * TILE + 2),
                           (float)(MAP_Y + ghosts[i].getY() * TILE + 2)});
        ghost.setFillColor(getGhostColor(ghosts[i].getId()));
        window.draw(ghost);
    }

    for (int p = 0; p < (int)players.size(); p++) {
        if (!players[p].isAlive()) continue;
        float radius = TILE / 2.f - 2.f;
        sf::CircleShape circle(radius);
        circle.setPosition({(float)(MAP_X + players[p].getX() * TILE + 2),
                            (float)(MAP_Y + players[p].getY() * TILE + 2)});
        circle.setFillColor(getPlayerColor(players[p].getPlayerId()));
        window.draw(circle);
    }

    int hudX = MAP_X + map.getWidth() * TILE + 20;
    int hudY = MAP_Y;

    sf::Text title(font, "SCORES", 20);
    title.setPosition({(float)hudX, (float)hudY});
    title.setFillColor(sf::Color::White);
    window.draw(title);

    for (int p = 0; p < (int)players.size(); p++) {
        std::string info = players[p].getName() + ": " + std::to_string(players[p].getScore());
        if (!players[p].isAlive()) info += " [DEAD]";

        sf::Text text(font, info, 16);
        text.setPosition({(float)hudX, (float)(hudY + 30 + p * 25)});
        text.setFillColor(getPlayerColor(players[p].getPlayerId()));
        window.draw(text);
    }
}
