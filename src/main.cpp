#include <array>
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "window.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "constants.hpp"

using std::vector, std::min, std::max, std::sin, std::cos, std::array, std::pair;

void DrawTriangle(vector<Color>& pixels, Triangle t) {
    const Vec3 a = t.a.position, b = t.b.position, c = t.c.position;
    int left = min(a.x, min(b.x, c.x));
    int right = max(a.x, max(b.x, c.x));
    int top = min(a.y, min(b.y, c.y));
    int bottom = max(a.y, max(b.y, c.y));

    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            auto [inside, wA, wB, wC] = InsideTriangle(t, x, y);
            if (!inside) continue;
            u8 R = wA * t.a.color.r + wB * t.b.color.r + wC * t.c.color.r;
            u8 G = wA * t.a.color.g + wB * t.b.color.g + wC * t.c.color.g;
            u8 B = wA * t.a.color.b + wB * t.b.color.b + wC * t.c.color.b;
            SetPixel(pixels, x, y, Color{R, G, B, 255});
        }
    }
}

Vertex3 ApplyTransform (Vertex3 v, Transform2D transform) {
    return {transform * v.position, v.color};
}

Vertex4 ApplyTransform (Vertex4 v, Transform3D transform) {
    return {transform * transform * v.position, v.color};
}

Triangle ApplyTransform(Triangle t, Transform2D transform) {
    Vertex3 avex = ApplyTransform(t.a, transform);
    Vertex3 bvex = ApplyTransform(t.b, transform);
    Vertex3 cvex = ApplyTransform(t.c, transform);

    return {avex, bvex, cvex };
}

Point3 ApplyTransform(Point3 p, Transform3D transform) {
    Vec4 temp {p.x, p.y, p.z, 1.0f};
    temp = transform * temp;
    return {temp.x, temp.y, temp.z};
}

Triangle Translate(Triangle t, float x, float y) {
    return ApplyTransform(t, Translation(x, y));
}

Point3 Translate(Point3 p, float x, float y, float z) {
    return ApplyTransform(p, Translation(x, y, z));
}

Triangle Rotate(Triangle t, Point2 pivot, float radians) {
    Transform2D to_pivot = Translation(-pivot.x, -pivot.y);
    Transform2D rotate = Rotation(radians);
    Transform2D to_original = Translation(pivot.x, pivot.y);
    Transform2D transform = Multiply(to_original, Multiply(rotate, to_pivot));

    return ApplyTransform(t, transform);
}

Triangle Rotate(Triangle t, float radians) {
    return Rotate(t, Centroid(t), radians);
}

Triangle Scale(Triangle t, float scale) {
    Point2 centroid = Centroid(t);
    Transform2D to_origin = Translation(-centroid.x, -centroid.y);
    Transform2D escalation = Escalation(scale);
    Transform2D to_original = Translation(centroid.x, centroid.y);
    Transform2D transform = Multiply(to_original, Multiply(escalation, to_origin));

    return ApplyTransform(t, transform);
}

Triangle Scale(Triangle t, float sx, float sy) {
    Point2 centroid = Centroid(t);
    Transform2D to_origin = Translation(-centroid.x, -centroid.y);
    Transform2D escalation = Escalation(sx, sy);
    Transform2D to_original = Translation(centroid.x, centroid.y);
    Transform2D transform = to_original * (escalation * to_origin);

    return ApplyTransform(t, transform);
}

void DrawTriangle(vector<Color> &pixels) {
    constexpr Vec3 v1 {525, 110, 1}, v2 {800, 75, 1}, v3 {700, 300, 1};
    constexpr Vertex3 vx1 {v1, green}, vx2 {v2, red}, vx3 {v3, purple};
    Triangle triangle {vx1, vx2, vx3};

    const float angle = SDL_GetTicks() * 0.005f;
    triangle = Rotate(triangle, angle);
    DrawTriangle(pixels, triangle);
}

Point3 RotatePointY(Point3 p, Point3 pivot, float radians) {
    Transform3D to_pivot = Translation(-pivot.x, -pivot.y, -pivot.z);
    Transform3D rotation = RotationY(radians);
    Transform3D to_original = Translation(pivot.x, pivot.y, pivot.z);
    Transform3D transform = to_original * (rotation * to_pivot);

    return ApplyTransform(p, transform);
}

Point3 RotateY(Point3 p, float radians) {
    return ApplyTransform(p, RotationY(radians));
}

void RotateCubeY(array<Point3, 8>& vertices, Point3 pivot, float radians) {
    for (Point3& p : vertices) {
        p = RotatePointY(p, pivot, radians);
    }
}

void DrawEdges(vector<Color>& pixels, array<Point3, 8>& vertices, array<pair<int, int>, 12>& edges, Color color) {
    for (auto& [v1, v2] : edges) {
        DrawLine(pixels, vertices[v1], vertices[v2], color);
    }
}

void DrawCube(vector<Color> &pixels) {
    Point3 p1 {-1, 1, 1};
    Point3 p2 {1, 1, 1};
    Point3 p3 {-1, -1, 1};
    Point3 p4 {1, -1, 1};
    Point3 p5 {-1, 1, -1};
    Point3 p6 {1, 1, -1};
    Point3 p7 {-1, -1, -1};
    Point3 p8 {1, -1, -1};

    array vertices {p1, p2, p3, p4, p5, p6, p7, p8};
    array<pair<int, int>, 12> edges { pair{0, 1}, {0, 2}, {1, 3}, {2, 3}, {4, 5}, {4, 6}, {5, 7}, {6, 7}, {0, 4}, {1, 5}, {2, 6}, {3, 7} };
    const float angle = SDL_GetTicks() * 0.005f;

    for (auto& p : vertices)
        p = RotateY(p, angle);

    for (auto& p : vertices)
        p = Translate(p, 0, 0, 5);

    for (auto& [v1, v2] : edges)
        DrawLine(pixels, vertices[v1], vertices[v2], red);
}

int main() {
    vector<Color> pixels(WIDTH * HEIGHT);
    auto [window, renderer, texture] = InitSDL();

    bool running = true;
    SDL_Event event;
    while (running) {
        Clear(pixels, black);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
        }

        DrawTriangle(pixels);
        DrawCube(pixels);
        Render(pixels, renderer, texture);
    }

    Cleanup(window, renderer, texture);
}
