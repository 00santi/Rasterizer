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
    Vec3 pos = v.position;
    return {
        Multiply(transform, pos),
        v.color
    };
}

Triangle ApplyTransform(Triangle t, Transform2D transform) {
    Vertex3 avex = ApplyTransform(t.a, transform);
    Vertex3 bvex = ApplyTransform(t.b, transform);
    Vertex3 cvex = ApplyTransform(t.c, transform);

    return {avex, bvex, cvex };
}

Triangle Translate(Triangle t, float x, float y) {
    return ApplyTransform(t, Translation(x, y));
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
    /*Transform3 to_pivot(-pivot.x, -pivot.y, -pivot.z);
    Transform3 rotation = Rotation(radians);
    Transform3 to_original(pivot.x, pivot.y, pivot.z);
    Transform3 transform = to_original * (rotation * to_pivot);

    return ApplyTransform(p, transform);*/
    return {0,0,0};
}

void RotateCubeY(array<Point3, 8>& vertices, Point3 pivot, float radians) {
    for (Point3& p : vertices) {
        p = RotatePointY(p, pivot, radians);
    }
}

void DrawCube(vector<Color> &pixels) {
    Point3 p1 = {-1, 1, 4};
    Point3 p2 = {1, 1, 4};
    Point3 p3 = {-1, -1, 4};
    Point3 p4 = {1, -1, 4};
    DrawLine(pixels, p1, p2, red);
    DrawLine(pixels, p1, p3, red);
    DrawLine(pixels, p2, p4, red);
    DrawLine(pixels, p3, p4, red);

    Point3 p5 = {-1, 1, 7};
    Point3 p6 = {1, 1, 7};
    Point3 p7 = {-1, -1, 7};
    Point3 p8 = {1, -1, 7};
    DrawLine(pixels, p5, p6, red);
    DrawLine(pixels, p5, p7, red);
    DrawLine(pixels, p6, p8, red);
    DrawLine(pixels, p7, p8, red);

    DrawLine(pixels, p1, p5, red);
    DrawLine(pixels, p2, p6, red);
    DrawLine(pixels, p3, p7, red);
    DrawLine(pixels, p4, p8, red);

    array<Point3, 8> vertices {p1, p2, p3, p4, p5, p6, p7, p8};
    array<pair<int, int>, 12> edges { pair{0, 1}, {0, 2}, {1, 3}, {2, 3}, {4, 5}, {4, 6}, {5, 7}, {6, 7}, {0, 4}, {1, 5}, {2, 6}, {3, 7} };

    RotateCubeY(vertices, {}, 0);
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
