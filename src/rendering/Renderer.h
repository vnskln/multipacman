#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

/// Klasa bazowa dla rendererow. Definiuje interfejs rysowania gry.
class Renderer {
public:
    Renderer() {}
    virtual ~Renderer() {}
    /// Rysuje aktualny stan gry.
    virtual void render(const Game& game) = 0;
};

#endif
