#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "window.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "constants.hpp"

using std::vector, std::min, std::max, std::abs, std::tuple;

void DrawLine(vector<Color>& pixels, float x0, float y0, float x1, float y1, Color color) {
    if (x0 >= WIDTH || y0 >= HEIGHT || x1 >= WIDTH || y1 >= HEIGHT) {
        std::cout << "Out of bounds\n";
        return;
    }

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

void DrawLine(vector<Color>& pixels, Point p1, Point p2, Color color) {
    DrawLine(pixels, p1.x, p1.y, p2.x, p2.y, color);
}

float EdgeFunction(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

float EdgeFunction(Point a, Point b, Point c) {
    return EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
}

tuple<bool, float, float, float> InsideTriangle(Triangle t, int px, int py) {
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
            if (!inside) continue;
            u8 R = wA * t.a.color.r + wB * t.b.color.r + wC * t.c.color.r;
            u8 G = wA * t.a.color.g + wB * t.b.color.g + wC * t.c.color.g;
            u8 B = wA * t.a.color.b + wB * t.b.color.b + wC * t.c.color.b;
            SetPixel(pixels, x, y, Color{R, G, B, 255});
        }
    }
}

Point Centroid(Triangle t) {
    float x = t.a.position.x + t.b.position.x + t.c.position.x;
    float y = t.a.position.y + t.b.position.y + t.c.position.y;
    x /= 3;
    y /= 3;
    return { x, y };
}

Triangle ApplyTranslation_Scale(Triangle t, Translation_Scale transform) {
    auto [Gx, Gy] = Centroid(t);

    float ax = Gx + transform.scale * (t.a.position.x - Gx);
    float ay = Gy + transform.scale * (t.a.position.y - Gy);
    float bx = Gx + transform.scale * (t.b.position.x - Gx);
    float by = Gy + transform.scale * (t.b.position.y - Gy);
    float cx = Gx + transform.scale * (t.c.position.x - Gx);
    float cy = Gy + transform.scale * (t.c.position.y - Gy);

    ax += transform.x;
    ay += transform.y;
    bx += transform.x;
    by += transform.y;
    cx += transform.x;
    cy += transform.y;

    return {
        {{ax, ay, 1}, t.a.color},
        {{bx, by, 1}, t.b.color},
        {{cx, cy, 1}, t.c.color},
    };
}

Triangle ApplyTransform(Triangle t, Transform transform) {
    Vec3 apos = t.a.position;
    Vec3 bpos = t.b.position;
    Vec3 cpos = t.c.position;

    apos = Multiply(transform, apos);
    bpos = Multiply(transform, bpos);
    cpos = Multiply(transform, cpos);

    Vertex avex {apos, t.a.color};
    Vertex bvex {bpos, t.b.color};
    Vertex cvex {cpos, t.c.color};

    return {avex, bvex, cvex, };
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

        Triangle vt {
            {{700, 25, 1}, red },
            {{400, 50, 1},green },
            {{600, 250, 1}, purple }
        };
        DrawTriangle(pixels, vt);

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    Cleanup(window, renderer, texture);
}
