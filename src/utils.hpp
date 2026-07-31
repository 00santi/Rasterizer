#pragma once
#include <iostream>
#include <vector>
#include "types.hpp"
#include "constants.hpp"

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

inline Point Centroid(Triangle t) {
    float x = t.a.position.x + t.b.position.x + t.c.position.x;
    float y = t.a.position.y + t.b.position.y + t.c.position.y;
    x /= 3;
    y /= 3;
    return { x, y };
}

inline float DegToRad(float degrees) {
    return degrees * 0.0174533f;
}

using std::vector, std::max, std::abs;
inline void DrawLine(vector<Color>& pixels, float x0, float y0, float x1, float y1, Color color) {
    const float dy = y1 - y0, dx = x1 - x0;
    const int steps = max(abs(dx), abs(dy));

    if (steps == 0) {
        SetPixel(pixels, x0, y0, color);
        return;
    }

    const float ystep = dy / steps;
    const float xstep = dx / steps;

    float y = y0, x = x0;
    for (int i = 0; i <= steps; i++) {
        SetPixel(pixels, x, y, color);
        y += ystep;
        x += xstep;
    }
}

inline void DrawLine(vector<Color>& pixels, Point p1, Point p2, Color color) {
    DrawLine(pixels, p1.x, p1.y, p2.x, p2.y, color);
}

inline float EdgeFunction(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

inline float EdgeFunction(Point a, Point b, Point c) {
    return EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
}

using std::tuple;
inline tuple<bool, float, float, float> InsideTriangle(Triangle t, int px, int py) {
    const Vec3 a = t.a.position, b = t.b.position, c = t.c.position;
    const float e0 = EdgeFunction(a.x, a.y, b.x, b.y, px, py);
    const float e1 = EdgeFunction(b.x, b.y, c.x, c.y, px, py);
    const float e2 = EdgeFunction(c.x, c.y, a.x, a.y, px, py);

    const bool inside = (e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0);
    const float area = EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
    if (area == 0 || !inside) return tuple{false, 0, 0, 0};
    return { inside, e1 / area, e2 / area, e0 / area };
}

inline Transform Translation(float x, float y) {
    return {
                {1, 0, x},
                {0, 1, y},
                {0, 0, 1}
    };
}

inline Transform Rotation(float radians) {
    return {
                {cos(radians), -sin(radians), 0},
                {sin(radians), cos(radians), 0},
                {0, 0, 1},
            };
}

inline Transform Escalation(float scale) {
    return {
                    {scale, 0, 0},
                    {0, scale, 0},
                    {0, 0, 1},
                };
}

inline Transform Escalation(float sx, float sy) {
    return {
                        {sx, 0, 0},
                        {0, sy, 0},
                        {0, 0, 1},
                    };
}
