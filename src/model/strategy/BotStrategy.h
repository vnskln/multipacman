#ifndef BOT_STRATEGY_H
#define BOT_STRATEGY_H

#include "Entity.h"
#include "Map.h"

class Player;
class Game;

/// Strategia AI dla bota-gracza. Idzie do najblizszej kropki, unika duchow.
class BotStrategy {
public:
    /// Wybiera kierunek ruchu dla bota na podstawie stanu gry.
    Direction chooseDirection(const Player& bot, const Game& game);

private:
    bool canMove(int x, int y, Direction dir, const Map& map) const;
    bool isGhostNearby(int x, int y, const Game& game, int range) const;
    Direction fleeDirection(const Player& bot, const Game& game);
};

#endif
