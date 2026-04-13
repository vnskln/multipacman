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
    std::vector<T> cells;

public:
    Grid(int width, int height, T defaultValue) {
        this->width = width;
        this->height = height;
        cells.resize(width * height, defaultValue);
    }
    
    T get(int x, int y) const {
        return cells[y * width + x];
    }

    void set(int x, int y, T value) {
        cells[y * width + x] = value;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }
};

#endif
