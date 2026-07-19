#pragma once
#include <SDL3/SDL.h>

void InitSDL();
SDL_Window* CreateWindow();
SDL_Renderer* CreateRenderer(SDL_Window* window);
SDL_Texture* CreateTexture(SDL_Renderer* renderer);
void Cleanup(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t);
