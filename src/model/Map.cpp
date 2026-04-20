#include "Map.h"
#include <fstream>

Map::Map() : grid(1, 1, CellType::Empty) {
}

void Map::parseLines(const std::vector<std::string>& lines) {
    if (lines.empty()) return;

    int height = (int)lines.size();
    int width = 0;
    for (int i = 0; i < height; i++) {
        if ((int)lines[i].size() > width) {
            width = (int)lines[i].size();
        }
    }
    if (width == 0) return;

    grid.reset(width, height, CellType::Empty);
    playerSpawns.clear();
    ghostSpawns.clear();
    pelletSpawns.clear();
    loadLayout(lines);
    layoutLines = lines;
}

bool Map::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) return false;

    parseLines(lines);

    std::string name = path;
    int lastSlash = -1;
    for (int i = 0; i < (int)name.size(); i++) {
        if (name[i] == '/' || name[i] == '\\') lastSlash = i;
    }
    if (lastSlash >= 0) name = name.substr(lastSlash + 1);
    int dot = -1;
    for (int i = 0; i < (int)name.size(); i++) {
        if (name[i] == '.') dot = i;
    }
    if (dot >= 0) name = name.substr(0, dot);
    mapName = name;

    return true;
}

void Map::loadFromLines(const std::vector<std::string>& lines, const std::string& name) {
    parseLines(lines);
    mapName = name;
}

void Map::loadLayout(const std::vector<std::string>& layout) {
    for (int y = 0; y < grid.getHeight() && y < (int)layout.size(); y++) {
        for (int x = 0; x < grid.getWidth() && x < (int)layout[y].size(); x++) {
            char c = layout[y][x];
            if (c == '#') {
                grid.set(x, y, CellType::Wall);
            } else if (c == 'P') {
                playerSpawns.push_back({x, y});
            } else if (c == 'G') {
                ghostSpawns.push_back({x, y});
            } else if (c == 'O') {
                pelletSpawns.push_back({x, y});
            }
        }
    }
}

bool Map::isWall(int x, int y) const {
    return grid.get(x, y) == CellType::Wall;
}

int Map::getWidth() const {
    return grid.getWidth();
}

int Map::getHeight() const {
    return grid.getHeight();
}

const std::vector<SpawnPoint>& Map::getPlayerSpawns() const {
    return playerSpawns;
}

const std::vector<SpawnPoint>& Map::getGhostSpawns() const {
    return ghostSpawns;
}

int Map::getMaxPlayers() const {
    return (int)playerSpawns.size();
}

const std::vector<SpawnPoint>& Map::getPelletSpawns() const {
    return pelletSpawns;
}

const std::string& Map::getMapName() const {
    return mapName;
}

const std::vector<std::string>& Map::getLayoutLines() const {
    return layoutLines;
}
