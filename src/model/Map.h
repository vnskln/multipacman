#ifndef MAP_H
#define MAP_H

#include "Grid.h"
#include <string>
#include <vector>

/// Typ komorki na mapie.
enum class CellType { Wall, Empty };

/// Pozycja startowa na mapie (spawn gracza lub ducha).
struct SpawnPoint {
    int x;
    int y;
};

/// Plansza gry. Ladowana z pliku (serwer) lub z linii tekstu (klient).
class Map {
private:
    Grid<CellType> grid;
    std::vector<SpawnPoint> playerSpawns;
    std::vector<SpawnPoint> ghostSpawns;
    std::string mapName;
    std::vector<std::string> layoutLines;

    void loadLayout(const std::vector<std::string>& layout);
    void parseLines(const std::vector<std::string>& lines);

public:
    Map();
    /// Laduje mape z pliku. Zwraca false jesli plik nie istnieje.
    bool loadFromFile(const std::string& path);
    /// Laduje mape z linii tekstu otrzymanych przez siec.
    void loadFromLines(const std::vector<std::string>& lines, const std::string& name);

    bool isWall(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    const std::vector<SpawnPoint>& getPlayerSpawns() const;
    const std::vector<SpawnPoint>& getGhostSpawns() const;
    /// Maksymalna liczba graczy na tej mapie (= liczba spawnow P).
    int getMaxPlayers() const;
    const std::string& getMapName() const;
    const std::vector<std::string>& getLayoutLines() const;
};

#endif
