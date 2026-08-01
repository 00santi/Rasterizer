#pragma once
#include <cstdint>

using u8 = std::uint8_t;

struct Color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
};

struct Point2 {
    float x = 0;
    float y = 0;
};

struct Point3 {
    float x = 0;
    float y = 0;
    float z = 0;
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

struct Mat3 {
    Vec3 a {};
    Vec3 b {};
    Vec3 c {};
};
using Transform = Mat3;

Vec3 Multiply(const Mat3& A, const Vec3& b);
Mat3 Multiply(const Mat3& A, const Mat3& B);
float Dot(const Vec3& a, const Vec3& b);
