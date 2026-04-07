#include "Map.h"

static const std::vector<std::string> DEFAULT_LAYOUT = {
    "###############",
    "#      #      #",
    "# #### # #### #",
    "#             #",
    "# ## ##### ## #",
    "#      #      #",
    "# ## ##### ## #",
    "#             #",
    "# #### # #### #",
    "#      #      #",
    "###############",
};

Map::Map() : grid(15, 11, CellType::Empty) {
    loadLayout(DEFAULT_LAYOUT);
}

void Map::loadLayout(const std::vector<std::string>& layout) {
    for (int y = 0; y < grid.getHeight() && y < (int)layout.size(); y++) {
        for (int x = 0; x < grid.getWidth() && x < (int)layout[y].size(); x++) {
            char c = layout[y][x];
            if (c == '#') {
                grid.set(x, y, CellType::Wall);
            }
        }
    }
}

bool Map::isWall(int x, int y) const {
    return grid.get(x, y) == CellType::Wall;
}

CellType Map::getCell(int x, int y) const {
    return grid.get(x, y);
}

int Map::getWidth() const {
    return grid.getWidth();
}

int Map::getHeight() const {
    return grid.getHeight();
}
