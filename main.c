#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <string.h>

#define SIZE 640
#define WHITE true
#define BLACK false

typedef enum {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	EMPTY,
} Piece;

typedef struct {
	bool side;
	Piece piece;
} Cell;

void exit_cleanup();

// Sprites

SDL_Texture** sprites;

// SDL Stuff

SDL_Window* window;
SDL_Renderer* renderer;

// Game abstraction

bool turn;
Cell board[8][8];
int selected[2];

// SDL Utility

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

// Game logic

void setup_board()
{
	for(int x = 0; x < 8; x++) {
		board[x][1].piece = PAWN;
		board[x][6].piece = PAWN;

		if(x == 0 || x == 7) {
			board[x][0].piece = ROOK;
			board[x][7].piece = ROOK;
		} else if(x == 1 || x == 6) {
			board[x][0].piece = KNIGHT;
			board[x][7].piece = KNIGHT;
		} else if(x == 2 || x == 5) {
			board[x][0].piece = BISHOP;
			board[x][7].piece = BISHOP;
		} else if(x == 3) {
			board[x][0].piece = QUEEN;
			board[x][7].piece = QUEEN;
		} else {
			board[x][0].piece = KING;
			board[x][7].piece = KING;
		}
	}

	for(int x = 0; x < 8; x++) {
		board[x][0].side = WHITE;
		board[x][1].side = WHITE;
		board[x][6].side = BLACK;
		board[x][7].side = BLACK;

		for(int y = 2; y < 6; y++) {
			board[x][y].piece = EMPTY;
		}
	}
}

// Draw functions

void draw_chessboard()
{
	SDL_Rect cell;
	SDL_RenderClear(renderer);
	cell.h = SIZE / 8;
	cell.w = SIZE / 8;

	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			bool x_even = !(x % 2);
			bool y_even = !(y % 2);
			cell.x = x * 80;
			cell.y = y * 80;

			if(selected[0] == x &&  7 - selected[1] == y)
				SDL_SetRenderDrawColor(renderer, 127, 255, 0, SDL_ALPHA_OPAQUE);
			else if((x_even && !y_even) || (!x_even && y_even))
				SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
			else
				SDL_SetRenderDrawColor(renderer, 250, 235, 215, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(renderer, &cell);
		}
	}
}

void draw_pieces()
{
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			if(board[x][y].piece == EMPTY)
				continue;
			SDL_Rect cell;
			int to_draw = board[x][y].side == WHITE ? 0 : 6;

			cell.h = SIZE / 8;
			cell.w = SIZE / 8;
			cell.x = x * 80;
			cell.y = (7 - y) * 80;

			to_draw += board[x][y].piece;
			if(SDL_RenderCopy(renderer, sprites[to_draw], NULL, &cell))
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't render piece");
		}
	}
}

void commit_frame()
{
	SDL_RenderPresent(renderer);
}

// Event and input handling

void check_input(SDL_MouseButtonEvent event)
{
	int x_s, y_s;
	if(event.button != SDL_BUTTON_LEFT)
		return;
	if(event.x < 0 || event.x > SIZE || event.y < 0 || event.y > SIZE)
		return;
	x_s = event.x / 80;
	y_s = (SIZE - event.y) / 80;
	selected[0] = x_s;
	selected[1] = y_s;
}

void check_event_queue()
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				exit_cleanup();
				exit(0);
			case SDL_MOUSEBUTTONDOWN:
				check_input(event.button);
			default:
				continue;
		}
	}
}

int main(int argc, char** argv)
{
	init_prog();
	load_sprites();

	setup_board();
	turn = WHITE;
	selected[0] = -1;
	selected[1] = -1;

	while(true) {
		draw_chessboard();
		draw_pieces();
		check_event_queue();
		commit_frame();
		SDL_Delay(16);
	}
}
