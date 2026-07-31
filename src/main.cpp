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

Transform Translation(float x, float y) {
    return {
            {1, 0, x},
            {0, 1, y},
            {0, 0, 1}
    };
}

Transform Rotation(float radians) {
    return {
            {cos(radians), -sin(radians), 0},
            {sin(radians), cos(radians), 0},
            {0, 0, 1},
        };
}

Transform Escalation(float scale) {
    return {
                {scale, 0, 0},
                {0, scale, 0},
                {0, 0, 1},
            };
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

int main() {
    vector<Color> pixels(WIDTH * HEIGHT);

    InitSDL();
    auto window = CreateWindow();
    auto renderer = CreateRenderer(window);
    auto texture = CreateTexture(renderer);

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

        Point topleft {0, 0};
        Point bottomright {799, 599};
        DrawLine(pixels, topleft, bottomright, red);

        Triangle triangle {
            {{300, 300, 1}, red},
            {{400, 400, 1}, red},
            {{250, 450, 1}, red}
        };
        DrawTriangle(pixels, triangle);

        Triangle triangle2 {
            {{700, 25, 1}, red },
            {{400, 50, 1},green },
            {{600, 250, 1}, purple }
        };
        DrawTriangle(pixels, triangle2);

        Triangle triangle3 = Translate(triangle2, -300, 10);
        triangle3 = Scale(triangle3, 0.7);
        DrawTriangle(pixels, triangle3);

        Triangle triangle4 = Translate(triangle3, 0, 300);
        triangle4 = Scale(triangle4, 1.3);
        triangle4 = Rotate(triangle4, DegToRad(115));
        DrawTriangle(pixels, triangle4);

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    Cleanup(window, renderer, texture);
}
