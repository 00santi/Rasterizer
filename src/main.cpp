#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "window.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "constants.hpp"

using std::vector, std::min, std::max, std::sin, std::cos;

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

Vertex ApplyTransform (Vertex v, Transform transform) {
    Vec3 pos = v.position;
    return {
        Multiply(transform, pos),
        v.color
    };
}

Triangle ApplyTransform(Triangle t, Transform transform) {
    Vertex avex = ApplyTransform(t.a, transform);
    Vertex bvex = ApplyTransform(t.b, transform);
    Vertex cvex = ApplyTransform(t.c, transform);

    return {avex, bvex, cvex };
}

Triangle Translate(Triangle t, float x, float y) {
    return ApplyTransform(t, Translation(x, y));
}

Triangle Rotate(Triangle t, Point pivot, float radians) {
    Transform to_pivot = Translation(-pivot.x, -pivot.y);
    Transform rotate = Rotation(radians);
    Transform to_original = Translation(pivot.x, pivot.y);
    Transform transform = Multiply(to_original, Multiply(rotate, to_pivot));

    return ApplyTransform(t, transform);
}

Triangle Rotate(Triangle t, float radians) {
    return Rotate(t, Centroid(t), radians);
}

Triangle Scale(Triangle t, float scale) {
    Point centroid = Centroid(t);
    Transform to_origin = Translation(-centroid.x, -centroid.y);
    Transform escalation = Escalation(scale);
    Transform to_original = Translation(centroid.x, centroid.y);
    Transform transform = Multiply(to_original, Multiply(escalation, to_origin));

    return ApplyTransform(t, transform);
}

Triangle Scale(Triangle t, float sx, float sy) {
    Point centroid = Centroid(t);
    Transform to_origin = Translation(-centroid.x, -centroid.y);
    Transform escalation = Escalation(sx, sy);
    Transform to_original = Translation(centroid.x, centroid.y);
    Transform transform = Multiply(to_original, Multiply(escalation, to_origin));

    return ApplyTransform(t, transform);
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

        DrawLine(pixels, {0, 0}, {WIDTH - 1, HEIGHT - 1}, red);

        constexpr Vertex v1 = {{525, 110, 1},green};
        constexpr Vertex v2 = {{800, 75, 1}, red};
        constexpr Vertex v3 = {{700, 300, 1}, purple};
        Triangle triangle {v1, v2, v3};
        const float angle = static_cast<float>(SDL_GetTicks()) * 0.005f;
        triangle = Rotate(triangle, angle);
        DrawTriangle(pixels, triangle);

        Render(pixels, renderer, texture);
    }

    Cleanup(window, renderer, texture);
}
