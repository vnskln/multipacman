#ifndef GRID_H
#define GRID_H

#include <vector>

/**
 * Dwuwymiarowa siatka przechowujaca elementy typu T.
 * Wewnetrznie uzywa jednowymiarowego wektora (y * width + x).
 * @tparam T typ przechowywanych elementow (np. CellType)
 */
template <typename T>
class Grid {
private:
    int width;
    int height;
    T defaultValue;
    std::vector<T> cells;

public:
    /// @param width szerokosc siatki
    /// @param height wysokosc siatki
    /// @param defaultValue wartosc poczatkowa komorek (zwracana tez przy get poza zakresem)
    Grid(int width, int height, T defaultValue) {
        this->width = width;
        this->height = height;
        this->defaultValue = defaultValue;
        cells.resize(width * height, defaultValue);
    }

    T get(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) return defaultValue;
        return cells[y * width + x];
    }

    void set(int x, int y, T value) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        cells[y * width + x] = value;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    /// Zmienia rozmiar siatki i wypelnia nowa wartoscia domyslna.
    void reset(int newWidth, int newHeight, T newDefaultValue) {
        width = newWidth;
        height = newHeight;
        defaultValue = newDefaultValue;
        cells.clear();
        cells.resize(width * height, newDefaultValue);
    }
};

#endif
