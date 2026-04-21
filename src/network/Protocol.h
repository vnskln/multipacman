#ifndef PROTOCOL_H
#define PROTOCOL_H

/**
 * Stale protokolu sieciowego.
 * Typy wiadomosci wysylanych miedzy serwerem a klientem
 * oraz kody kierunkow do przesylania inputu.
 */

#include <cstdint>

const unsigned short SERVER_PORT = 53000;
const int MAX_PLAYERS = 4;

/// Wiadomosci od klienta do serwera.
const std::int32_t MSG_JOIN = 1;
const std::int32_t MSG_PLAYER_INPUT = 2;
const std::int32_t MSG_START_GAME = 3;
const std::int32_t MSG_ADD_BOT = 4;
const std::int32_t MSG_CHANGE_MAP = 5;
const std::int32_t MSG_CHANGE_DIFFICULTY = 6;

/// Wiadomosci od serwera do klienta.
const std::int32_t MSG_JOIN_ACCEPTED = 10;
const std::int32_t MSG_JOIN_REJECTED = 11;
const std::int32_t MSG_LOBBY_UPDATE = 12;
const std::int32_t MSG_GAME_STATE = 13;
const std::int32_t MSG_GAME_STARTED = 14;
const std::int32_t MSG_GAME_OVER = 15;

/// Poziomy trudnosci (wplywaja na prędkosc duchow).
const std::int32_t DIFFICULTY_EASY = 0;
const std::int32_t DIFFICULTY_NORMAL = 1;
const std::int32_t DIFFICULTY_HARD = 2;

/// Kody kierunkow przesylane w MSG_PLAYER_INPUT.
const std::int32_t DIR_UP = 0;
const std::int32_t DIR_DOWN = 1;
const std::int32_t DIR_LEFT = 2;
const std::int32_t DIR_RIGHT = 3;
const std::int32_t DIR_NONE = -1;

#endif
