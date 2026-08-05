#pragma once
#include <iostream>
#include <vector>
#include "types.hpp"
#include "constants.hpp"

inline int Idx(const int x, const int y) {
    return y * WIDTH + x;
}

inline void Clear(std::vector<Color>& pixels, const Color color) {
    for (auto& pixel : pixels)
        pixel = color;
}

inline void SetPixel(std::vector<Color>& pixels, const int x, const int y, const Color color) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        std::cout << "Out of bounds, x: " << x << ", y: " << y << '\n';
    else
        pixels[Idx(x, y)] = color;
}

inline void SetPixel(std::vector<Color>& pixels, const Point2 p, const Color color) {
    SetPixel(pixels, p.x, p.y, color);
}

inline Point2 Centroid(const Triangle& t) {
    float x = t.a.position.x + t.b.position.x + t.c.position.x;
    float y = t.a.position.y + t.b.position.y + t.c.position.y;
    x /= 3;
    y /= 3;
    return { x, y };
}

inline float DegToRad(const float degrees) {
    return degrees * 0.0174533f;
}

inline Point2 Project(const float x, const float y, const float z) {
    constexpr float scale = 300.0f;

    return {
        WIDTH / 2 + (x / z) * scale,
        HEIGHT / 2 - (y / z) * scale
    };
}

inline Point2 Project(const Point3& p) {
    return Project(p.x, p.y, p.z);
}

inline Point2 Project(const Vec4& p) {
    return Project(p.x, p.y, p.z);
}

using std::vector, std::max, std::min, std::abs;
inline void DrawLine(vector<Color>& pixels, const Point2& a, const Color& a_color, const Point2& b, const Color& b_color) {
    const float dy = b.y - a.y, dx = b.x - a.x;
    const int steps = max(abs(dx), abs(dy));

    if (steps == 0) {
        const Color color = a_color * 0.5 + b_color * 0.5;
        SetPixel(pixels, a.x, a.y, color);
        return;
    }

    const float ystep = dy / steps;
    const float xstep = dx / steps;

    float y = a.y, x = a.x;
    for (int i = 0; i <= steps; i++) {
        const float t = static_cast<float>(i) / steps;
        const Color color = a_color * (1 - t) + b_color * t;
        SetPixel(pixels, x, y, color);
        y += ystep;
        x += xstep;
    }
}

inline void DrawLine(vector<Color>& pixels, const Vertex4& a, const Vertex4& b) {
    DrawLine(pixels, Project(a.position), a.color, Project(b.position), b.color);
}

inline float EdgeFunction(const float ax, const float ay, const float bx, const float by, const float px, const float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

inline float EdgeFunction(const Point2 a, const Point2 b, const Point2 c) {
    return EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
}

using std::tuple;
inline tuple<bool, float, float, float> InsideTriangle(const Triangle& t, const float px, const float py) {
    const Vec3 a = t.a.position, b = t.b.position, c = t.c.position;
    const float e0 = EdgeFunction(a.x, a.y, b.x, b.y, px, py);
    const float e1 = EdgeFunction(b.x, b.y, c.x, c.y, px, py);
    const float e2 = EdgeFunction(c.x, c.y, a.x, a.y, px, py);

    const bool inside = (e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0);
    const float area = EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
    if (area == 0 || !inside) return tuple{false, 0, 0, 0};
    return { inside, e1 / area, e2 / area, e0 / area };
}

void DrawTriangle(vector<Color>& pixels, Triangle t) {
    const Vec3 a = t.a.position, b = t.b.position, c = t.c.position;
    int left = min(a.x, min(b.x, c.x));
    int right = max(a.x, max(b.x, c.x));
    int top = min(a.y, min(b.y, c.y));
    int bottom = max(a.y, max(b.y, c.y));

    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            auto [inside, wA, wB, wC] = InsideTriangle(t, x, y);
            if (!inside)
                continue;
            const Color color = wA * t.a.color + wB * t.b.color + wC * t.c.color;
            SetPixel(pixels, x, y, color);
        }
    }
}

inline Transform2D Translation(const float x, const float y) {
    return {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    };
}

inline Transform2D Rotation(const float radians) {
    return {
        {cos(radians), -sin(radians), 0},
        {sin(radians), cos(radians), 0},
        {0, 0, 1}
    };
}

inline Transform2D Scale(const float sx, const float sy) {
    return {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    };
}

inline Transform3D Translation(const float x, const float y, const float z) {
    return {
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1}
    };
}

inline Transform3D RotationX(const float radians) {
    return {
        {1, 0, 0, 0},
        {0, cos(radians), sin(radians), 0},
        {0, -sin(radians), cos(radians), 0},
        {0, 0, 0, 1}
    };
}

inline Transform3D RotationY(const float radians) {
    return {
        {cos(radians), 0, -sin(radians), 0},
        {0, 1, 0, 0},
        {sin(radians), 0, cos(radians), 0},
        {0, 0, 0, 1}
    };
}

inline Transform3D RotationZ(const float radians) {
    return {
        {cos(radians), sin(radians), 0, 0},
        {-sin(radians), cos(radians), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

inline Transform3D Scale(const float sx, const float sy, const float sz) {
    return {
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1}
    };
}

inline Transform3D View(const Camera& c) {
    return Translation(-c.position.x,
                       -c.position.y,
                       -c.position.z);
}
