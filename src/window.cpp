#include <iostream>
#include "window.hpp"
#include "types.hpp"

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

void Cleanup(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t) {
    SDL_DestroyWindow(w);
    SDL_DestroyRenderer(r);
    SDL_DestroyTexture(t);
    SDL_Quit();
}
