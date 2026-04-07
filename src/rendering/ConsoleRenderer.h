#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "Renderer.h"

class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) override;
};

#endif
