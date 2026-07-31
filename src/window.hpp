#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "types.hpp"

std::tuple<SDL_Window*, SDL_Renderer*, SDL_Texture*> InitSDL();
SDL_Window* CreateWindow();
SDL_Renderer* CreateRenderer(SDL_Window* window);
SDL_Texture* CreateTexture(SDL_Renderer* renderer);
void Cleanup(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t);
void Render(const std::vector<Color>& pixels, SDL_Renderer* renderer, SDL_Texture* texture);