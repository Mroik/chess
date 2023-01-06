#include "include/draw.h"
#include "include/game.h"
#include "include/input.h"
#include "include/utils.h"

extern bool turn;
extern int selected[2];

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
