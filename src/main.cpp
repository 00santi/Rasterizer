#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "window.h"

using u8 = uint8_t;
constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

struct Color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
};

int Idx(int x, int y) {
    return y * WIDTH + x;
}

using std::vector;
void SetPixel(vector<Color>& pixels, int x, int y, Color color) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        std::cout << "Out of bounds, x: " << x << ", y: " << y << std::endl;
        return;
    }
    pixels[Idx(x, y)] = color;
}

void DrawLine(vector<Color>& pixels, int x0, int y0, int x1, int y1, Color color) {
    using std::max, std::abs;
    if (x0 >= WIDTH || y0 >= HEIGHT || x1 >= WIDTH || y1 >= HEIGHT) {
        std::cout << "Out of bounds\n";
        return;
    }

    float dy = y1 - y0, dx = x1 - x0;
    int steps = max(abs(dx), abs(dy));

    if (steps == 0) {
        SetPixel(pixels, x0, y0, color);
        return;
    }

    dy /= steps;
    dx /= steps;

    float y = y0, x = x0;
    for (int i = 0; i <= steps; i++) {
        SetPixel(pixels, x, y, color);
        y += dy;
        x += dx;
    }
}

struct Triangle {
    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

int EdgeFunction(int ax, int ay, int bx, int by, int px, int py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

bool InsideTriangle(Triangle t, int px, int py) {
    int e0 = EdgeFunction(t.x0, t.y0, t.x1, t.y1, px, py);
    int e1 = EdgeFunction(t.x1, t.y1, t.x2, t.y2, px, py);
    int e2 = EdgeFunction(t.x2, t.y2, t.x0, t.y0, px, py);

    return (e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0);
}

void DrawTriangle(vector<Color>& pixels, Triangle t, Color color) {
    using std::max, std::abs, std::min;

    int left = min(t.x0, min(t.x1, t.x2));
    int right = max(t.x0, max(t.x1, t.x2));
    int top = min(t.y0, min(t.y1, t.y2));
    int bottom = max(t.y0, max(t.y1, t.y2));

    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++)
            if (InsideTriangle(t, x, y))
                SetPixel(pixels, x, y, color);
    }
}

void Clear(vector<Color>& pixels, Color color) {
    for (auto& pixel : pixels)
        pixel = color;
}

int main() {
    vector<Color> pixels(WIDTH * HEIGHT);

    InitSDL();
    auto window = CreateWindow();
    auto renderer = CreateRenderer(window);
    auto texture = CreateTexture(renderer);

    Color green {28, 232, 179, 255};
    Clear(pixels, green);

    bool running = true;
    SDL_Event event;
    while (running) {
        auto black = Color {0, 0, 0, 255};
        Clear(pixels, black);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
        }

        auto red = Color {150, 0, 0, 255};
        auto triangle = Triangle{300, 300, 400, 400, 250, 450};
        DrawLine(pixels, 0, 0, 799, 599, red);
        DrawTriangle(pixels, triangle, red);

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    Cleanup(window, renderer, texture);
}
