#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/game.h"
#include "include/utils.h"

SDL_Texture** sprites;
SDL_Window* window;
SDL_Renderer* renderer;

void init_prog()
{
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "There was a problem initializing SDL2");
		exit(1);
	}

	if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL2_image");
		SDL_Quit();
		exit(1);
	}

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
	if(!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "There was an error creating the window");
		IMG_Quit();
		SDL_Quit();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "There was an error creating the window");
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		exit(1);
	}
}

void load_sprites()
{
	sprites = malloc(sizeof(SDL_Texture*) * 12);
	for(int x = 0; x < 12; x++)
		sprites[x] = NULL;

	sprites[PAWN] = IMG_LoadTexture(renderer, "assets/white_pawn.svg");
	if(!sprites[PAWN]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load pawn.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[KNIGHT] = IMG_LoadTexture(renderer, "assets/white_knight.svg");
	if(!sprites[KNIGHT]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load knight.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[BISHOP] = IMG_LoadTexture(renderer, "assets/white_bishop.svg");
	if(!sprites[BISHOP]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load bishop.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[ROOK] = IMG_LoadTexture(renderer, "assets/white_rook.svg");
	if(!sprites[ROOK]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load rook.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[QUEEN] = IMG_LoadTexture(renderer, "assets/white_queen.svg");
	if(!sprites[QUEEN]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load queen.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[KING] = IMG_LoadTexture(renderer, "assets/white_king.svg");
	if(!sprites[KING]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load king.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + PAWN] = IMG_LoadTexture(renderer, "assets/black_pawn.svg");
	if(!sprites[PAWN]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load pawn.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + KNIGHT] = IMG_LoadTexture(renderer, "assets/black_knight.svg");
	if(!sprites[KNIGHT]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load knight.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + BISHOP] = IMG_LoadTexture(renderer, "assets/black_bishop.svg");
	if(!sprites[BISHOP]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load bishop.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + ROOK] = IMG_LoadTexture(renderer, "assets/black_rook.svg");
	if(!sprites[ROOK]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load rook.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + QUEEN] = IMG_LoadTexture(renderer, "assets/black_queen.svg");
	if(!sprites[QUEEN]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load queen.svg");
		exit_cleanup();
		exit(2);
	}

	sprites[6 + KING] = IMG_LoadTexture(renderer, "assets/black_king.svg");
	if(!sprites[KING]) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load king.svg");
		exit_cleanup();
		exit(2);
	}
}

void destroy_sprites()
{
	for(int x = 0; x < 12; x++) {
		if(sprites[x]) {
			SDL_DestroyTexture(sprites[x]);
		}
	}
	free(sprites);
}

void exit_cleanup()
{
	destroy_sprites();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}
