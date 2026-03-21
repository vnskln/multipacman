#ifndef GRID_H
#define GRID_H

#include <vector>

template <typename T>
class Grid {
private:
    int width;
    int height;
    T defaultValue;
    std::vector<T> cells;

public:
    Grid(int width, int height, T defaultValue) {
        this->width = width;
        this->height = height;
        this->defaultValue = defaultValue;
        cells.resize(width * height, defaultValue);
    }
    
    T get (int x, int y) {
        return cells[y * width + x];
    }

    void set (int x, int y, T value) {
        cells[y * width + x] = value;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }
    
    T getDefaultValue() {
        return defaultValue;
    }
};

#endif
