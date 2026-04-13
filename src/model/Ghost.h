#ifndef GHOST_H
#define GHOST_H

#include "Entity.h"

/// Duch scigajacy graczy. Porusza sie co drugi tick.
class Ghost : public Entity {
private:
    int id;

public:
    Ghost(int x, int y, int id);

    char getSymbol() const override;

    int getId() const;
};

#endif
