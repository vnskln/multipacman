#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include "Ghost.h"
#include "Dot.h"
#include "GhostStrategy.h"
#include "BotStrategy.h"
#include <vector>
#include <memory>

/**
 * Glowna logika gry - przechowuje mape, graczy, duchy i kropki.
 * Serwer wywoluje update() co tick, klient tylko odczytuje stan.
 */
class Game {
private:
    Map map;
    std::vector<Player> players;
    std::vector<Ghost> ghosts;
    std::vector<Dot> dots;
    std::vector<std::unique_ptr<GhostStrategy>> strategies;
    BotStrategy botStrategy;
    bool gameOver;
    bool playerWon;
    int tickCount;
    int ghostTickDivisor;
    int stuckTicks;

    void createGhosts();
    void spawnDots();
    void moveBots();
    void movePlayers();
    void moveGhosts();
    void collectDots();
    void checkCollisions();
    void checkWin();

public:
    Game();
    /// Tworzy gre z mapa zaladowana z pliku (serwer).
    Game(const std::string& mapFile);
    /// Tworzy gre z linii layoutu otrzymanych przez siec (klient).
    Game(const std::vector<std::string>& mapLines, const std::string& mapName);
    /// @param playerId unikalny identyfikator gracza (z serwera)
    /// @param name nazwa wyswietlana w lobby i rankingu
    void addPlayer(int playerId, const std::string& name);
    /// @param playerId unikalny identyfikator bota (z serwera)
    /// @param name nazwa wyswietlana (np. "Bot 1")
    void addBot(int playerId, const std::string& name);
    /// Dodaje ducha o podanym id i pozycji (klient, dane z serwera).
    void addGhost(int id, int x, int y);
    /// Dodaje kropke o podanej pozycji (klient, dane z serwera).
    void addDot(int x, int y, bool powerPellet);
    void removePlayer(int playerId);
    /// Rozpoczyna gre - tworzy kropki na mapie.
    void start();
    /// Ustawia kierunek ruchu gracza o danym id.
    void handleInput(int playerId, Direction dir);
    /// Jeden tick gry: ruch, zbieranie, kolizje, warunek wygranej.
    void update();

    const Map& getMap() const;
    const std::vector<Player>& getPlayers() const;
    const std::vector<Ghost>& getGhosts() const;
    const std::vector<Dot>& getDots() const;

    std::vector<Player>& getPlayers();
    std::vector<Ghost>& getGhosts();
    std::vector<Dot>& getDots();
    void setOver(bool over, bool won);
    /// Ustawia co ile tickow rusza sie duch (1=szybko, 2=normalnie, 3=wolno).
    void setGhostTickDivisor(int n);

    bool isOver() const;
    bool isWon() const;
};

#endif
