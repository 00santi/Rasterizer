#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "window.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::vector, std::min, std::max, std::abs;

void DrawLine(vector<Color>& pixels, int x0, int y0, int x1, int y1, Color color) {
    if (x0 >= WIDTH || y0 >= HEIGHT || x1 >= WIDTH || y1 >= HEIGHT) {
        std::cout << "Out of bounds\n";
        return;
    }

    const int dy = y1 - y0, dx = x1 - x0;
    const int steps = max(abs(dx), abs(dy));

    if (steps == 0) {
        SetPixel(pixels, x0, y0, color);
        return;
    }

    const float ystep = static_cast<float>(dy) / steps;
    const float xstep = static_cast<float>(dx) / steps;

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

int EdgeFunction(int ax, int ay, int bx, int by, int px, int py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

int EdgeFunction(Point a, Point b, Point c) {
    return EdgeFunction(a.x, a.y, b.x, b.y, c.x, c.y);
}

bool InsideTriangle(Triangle t, int px, int py) {
    const Point a = t.a, b = t.b, c = t.c;
    int e0 = EdgeFunction(a.x, a.y, b.x, b.y, px, py);
    int e1 = EdgeFunction(b.x, b.y, c.x, c.y, px, py);
    int e2 = EdgeFunction(c.x, c.y, a.x, a.y, px, py);

    return (e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0);
}

void DrawTriangle(vector<Color>& pixels, Triangle t, Color color) {
    const Point a = t.a, b = t.b, c = t.c;
    int left = min(a.x, min(b.x, c.x));
    int right = max(a.x, max(b.x, c.x));
    int top = min(a.y, min(b.y, c.y));
    int bottom = max(a.y, max(b.y, c.y));

    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++)
            if (InsideTriangle(t, x, y))
                SetPixel(pixels, x, y, color);
    }
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

        Triangle triangle {300, 300, 400, 400, 250, 450};
        Point topleft {0, 0};
        Point bottomright {799, 599};
        DrawLine(pixels, topleft, bottomright, red);
        DrawTriangle(pixels, triangle, red);

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    Cleanup(window, renderer, texture);
}
