#include <stdbool.h>
#include <SDL2/SDL.h>
#include "include/utils.h"
#include "include/game.h"
#include "include/draw.h"

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
			cell.x = x * SQUARE_SIZE;
			cell.y = y * SQUARE_SIZE;

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
			cell.x = x * SQUARE_SIZE;
			cell.y = (7 - y) * SQUARE_SIZE;

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
