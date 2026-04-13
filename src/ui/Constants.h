#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int WINDOW_WIDTH = 1200;
static const int WINDOW_HEIGHT = 700;
static const float CENTER_X = WINDOW_WIDTH / 2.f;

static const int MAP_COLS = 21;
static const int MAP_ROWS = 15;
static const int MAP_MARGIN = 20;
static const int HUD_WIDTH = 200;

static const int TILE_BY_W = (WINDOW_WIDTH - HUD_WIDTH - 2 * MAP_MARGIN) / MAP_COLS;
static const int TILE_BY_H = (WINDOW_HEIGHT - 2 * MAP_MARGIN) / MAP_ROWS;
static const int TILE = (TILE_BY_W < TILE_BY_H) ? TILE_BY_W : TILE_BY_H;

#endif
