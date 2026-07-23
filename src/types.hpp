#pragma once
#include <cstdint>

using u8 = std::uint8_t;

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

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Vertex {
    Vec3 position;
    Color color{};
};

struct Triangle {
    Vertex a {};
    Vertex b {};
    Vertex c {};
};

using Mat3 = float[3][3];
using Transform = Mat3;

Vec3 Multiply(Mat3 a, Vec3 b);
