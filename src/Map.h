#ifndef MAP_H
#define MAP_H

#include "Grid.h"
#include <string>
#include <vector>

enum class CellType { Wall, Empty };

class Map {
private:
    Grid<CellType> grid;
    void loadLayout(const std::vector<std::string>& layout);

public:
    Map();
    bool isWall(int x, int y) const;
    CellType getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
};

#endif
