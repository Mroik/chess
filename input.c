#include <SDL2/SDL.h>
#include "include/game.h"
#include "include/utils.h"

void check_input(SDL_MouseButtonEvent event)
{
	int x_s, y_s;
	if(event.button == SDL_BUTTON_RIGHT) {
		selected[0] = -1;
		selected[1] = -1;
		return;
	}
	if(event.button != SDL_BUTTON_LEFT)
		return;
	if(event.x < 0 || event.x > SIZE || event.y < 0 || event.y > SIZE)
		return;

	x_s = event.x / SQUARE_SIZE;
	y_s = (SIZE - event.y) / SQUARE_SIZE;
	if(selected[0] == -1 && (turn != board[x_s][y_s].side || board[x_s][y_s].piece == EMPTY))
		return;

	if(selected[0] == -1) {
		selected[0] = x_s;
		selected[1] = y_s;
	} else {
		make_move(selected[0], selected[1], x_s, y_s);
	}
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
				break;
		}
	}
}
