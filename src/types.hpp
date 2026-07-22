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
    float x = 0;
    float y = 0;
};

struct Vertex {
    float x = 0;
    float y = 0;
    Color color{};
};

struct Triangle {
    Vertex a {};
    Vertex b {};
    Vertex c {};
};

struct Transform {
    float x = 0;
    float y = 0;
    float scale = 1.0f;
};

inline constexpr Color green {28, 232, 179, 255};
inline constexpr Color black {0, 0, 0, 255};
inline constexpr Color red {150, 0, 0, 255};
inline constexpr Color white {255, 255, 255, 255};
inline constexpr Color purple {148, 62, 143, 255};
