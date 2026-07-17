#include <iostream>
#include <vector>
#include <SDL3/SDL.h>

using u8 = uint8_t;
constexpr size_t WIDTH = 800;
constexpr size_t HEIGHT = 600;

void InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO))
        return;
    std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    std::exit(1);
}

SDL_Window* CreateWindow() {
    auto window = SDL_CreateWindow(
        "C++ Rasterizer Window",
        WIDTH,
        HEIGHT,
        0
    );
    if (window)
        return window;
    std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    std::exit(1);
}

void Cleanup(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t) {
    SDL_DestroyWindow(w);
    SDL_DestroyRenderer(r);
    SDL_DestroyTexture(t);
    SDL_Quit();
}

SDL_Renderer* CreateRenderer(SDL_Window* window) {
    auto renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer)
        return renderer;
    std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
    std::exit(1);
}

SDL_Texture* CreateTexture(SDL_Renderer* renderer) {
    auto texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );
    if (texture)
        return texture;
    std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    std::exit(1);
}

struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

size_t Idx(size_t x, size_t y) {
    return y * WIDTH + x;
}

using std::vector;
void SetPixel(vector<Color>& pixels, size_t x, size_t y, Color color) {
    if (x >= WIDTH || y >= HEIGHT) {
        std::cout << "Out of bounds\nx: " << x << "\ny: " << y << std::endl;
        return;
    }
    pixels[Idx(x, y)] = color;
}

int main() {
    vector<Color> pixels(WIDTH * HEIGHT);

    InitSDL();
    auto window = CreateWindow();
    auto renderer = CreateRenderer(window);
    auto texture = CreateTexture(renderer);

    Color my_color {28, 232, 179, 255};
    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
        pixels[i] = my_color;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
        }
        SetPixel(pixels, 400, 300, Color {255, 0, 0, 255});
        SetPixel(pixels, 0, 0, Color {255, 0, 0, 255});
        SetPixel(pixels, 0, 599, Color {255, 0, 0, 255});
        SetPixel(pixels, 799, 0, Color {255, 0, 0, 255});
        SetPixel(pixels, 799, 599, Color {255, 0, 0, 255});

        for (size_t i = 0; i < HEIGHT; i++) {
            SetPixel(pixels, i, i, Color {255, 0, 0, 255});
        }

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    Cleanup(window, renderer, texture);
}
