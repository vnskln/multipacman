#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "Renderer.h"

/// Renderer rysujacy gre w konsoli (ASCII).
class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) override;
};

#endif
