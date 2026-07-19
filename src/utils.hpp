#pragma once
#include <iostream>
#include <vector>
#include "types.hpp"

inline int Idx(int x, int y) {
    return y * WIDTH + x;
}

inline void Clear(std::vector<Color>& pixels, const Color color) {
    for (auto& pixel : pixels)
        pixel = color;
}

inline void SetPixel(std::vector<Color>& pixels, const int x, const int y, const Color color) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        std::cout << "Out of bounds, x: " << x << ", y: " << y << std::endl;
        return;
    }
    pixels[Idx(x, y)] = color;
}

inline void SetPixel(std::vector<Color>& pixels, const Point p, const Color color) {
    SetPixel(pixels, p.x, p.y, color);
}
