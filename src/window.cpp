#include <iostream>
#include "window.hpp"
#include "constants.hpp"

std::tuple<SDL_Window*, SDL_Renderer*, SDL_Texture*> InitSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    auto window = CreateWindow();
    auto renderer = CreateRenderer(window);
    auto texture = CreateTexture(renderer);

    return {window, renderer, texture};
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

void Render(const std::vector<Color>& pixels, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(Color));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}