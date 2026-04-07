#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

class Renderer {
public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void render(const Game& game) = 0;
};

#endif
