#pragma once

using u8 = uint8_t;
inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 600;

struct Color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
};

struct Point {
    int x = 0;
    int y = 0;
};

struct Triangle {
    Point a {};
    Point b {};
    Point c {};
};

struct Vertex {
    int x = 0;
    int y = 0;
    Color color{};
};

inline constexpr Color green {28, 232, 179, 255};
inline constexpr Color black {0, 0, 0, 255};
inline constexpr Color red {150, 0, 0, 255};
