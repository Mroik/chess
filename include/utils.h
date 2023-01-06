#pragma once

#include <SDL2/SDL.h>

#define SIZE 640
#define SQUARE_SIZE SIZE/8

extern SDL_Texture** sprites;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

void init_prog();
void load_sprites();
void destroy_sprites();
void exit_cleanup();
