#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <string.h>

#define SIZE 640
#define SQUARE_SIZE SIZE/8
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
int w_x, w_y;
int b_x, b_y;
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
	w_x = 4;
	w_y = 0;
	b_x = 4;
	b_y = 7;
}

bool check_pawn_moveset(int from_x, int from_y, int to_x, int to_y)
{
	// TODO en passe
	if(board[from_x][from_y].side == WHITE) {
		if(from_x == to_x && from_y == 1 && to_y == 3 && board[to_x][to_y].piece == EMPTY)
			return true;
		if(to_x < from_x - 1 || to_x > from_x + 1 || from_y + 1 != to_y)
			return false;
		if(to_x == from_x && board[to_x][to_y].piece != EMPTY)
			return false;
		if(to_x != from_x && (board[to_x][to_y].piece == EMPTY || board[to_x][to_y].side == WHITE))
			return false;
	} else {
		if(from_x == to_x && from_y == 6 && to_y == 4 && board[to_x][to_y].piece == EMPTY)
			return true;
		if(to_x < from_x -1 || to_x > from_x + 1 || from_y - 1 != to_y)
			return false;
		if(to_x == from_x && board[to_x][to_y].piece != EMPTY)
			return false;
		if(to_x != from_x && (board[to_x][to_y].piece == EMPTY || board[to_x][to_y].side == BLACK))
			return false;
	}
	return true;
}

bool check_knight_moveset(int from_x, int from_y, int to_x, int to_y)
{
	if(board[to_x][to_y].piece != EMPTY && board[to_x][to_y].side == board[from_x][from_y].side)
		return false;
	if(from_x + 1 == to_x && from_y + 2 == to_y)
		return true;
	if(from_x + 2 == to_x && from_y + 1 == to_y)
		return true;
	if(from_x + 2 == to_x && from_y - 1 == to_y)
		return true;
	if(from_x + 1 == to_x && from_y - 2 == to_y)
		return true;
	if(from_x - 1 == to_x && from_y - 2 == to_y)
		return true;
	if(from_x - 2 == to_x && from_y - 1 == to_y)
		return true;
	if(from_x - 2 == to_x && from_y + 1 == to_y)
		return true;
	if(from_x - 1 == to_x && from_y + 2 == to_y)
		return true;
	return false;
}

bool check_bishop_moveset(int from_x, int from_y, int to_x, int to_y)
{
	bool ne = true, se = true, sw = true, nw = true;
	if(abs(from_x - to_x) != abs(from_y - to_y))
		return false;
	if(board[to_x][to_y].piece != EMPTY && board[to_x][to_y].side == board[from_x][from_y].side)
		return false;

	for(int i = 1; i <= 7; i++) {
		if(ne && from_x + i == to_x && from_y + i == to_y)
			return true;
		else if(ne && board[from_x + i][from_y + i].piece != EMPTY)
			ne = false;

		if(se && from_x + i == to_x && from_y - i == to_y)
			return true;
		else if(se && board[from_x + i][from_y - i].piece != EMPTY)
			se = false;

		if(sw && from_x - i == to_x && from_y - i == to_y)
			return true;
		else if(sw && board[from_x - i][from_y - i].piece != EMPTY)
			sw = false;

		if(nw && from_x - i == to_x && from_y + i == to_y)
			return true;
		else if(nw && board[from_x - i][from_y + i].piece != EMPTY)
			nw = false;
	}
	return false;
}

bool check_rook_moveset(int from_x, int from_y, int to_x, int to_y)
{
	bool n = true, e = true, s = true, w = true;
	if(from_x != to_x && from_y != to_y)
		return false;
	if(board[to_x][to_y].piece != EMPTY && board[to_x][to_y].side == board[from_x][from_y].side)
		return false;

	for(int i = 1; i <= 7; i++) {
		if(n && from_y + i == to_y)
			return true;
		else if(n && board[from_x][from_y + i].piece != EMPTY)
			n = false;

		if(e && from_x + i == to_x)
			return true;
		else if(e && board[from_x + i][from_y].piece != EMPTY)
			e = false;

		if(s && from_y - i == to_y)
			return true;
		else if(s && board[from_x][from_y - i].piece != EMPTY)
			s = false;

		if(w && from_x - i == to_x)
			return true;
		else if(w && board[from_x - i][from_y].piece != EMPTY)
			w = false;
	}
	return false;
}

bool check_king_moveset(int from_x, int from_y, int to_x, int to_y)
{
	// TODO Implement castling
	if(board[to_x][to_y].piece != EMPTY && board[to_x][to_y].side == board[from_x][from_y].side)
		return false;

	if(from_x + 1 == to_x && from_y == to_y)
		return true;
	else if(from_x + 1 == to_x && from_y - 1 == to_y)
		return true;
	else if(from_x == to_x && from_y - 1 == to_y)
		return true;
	else if(from_x - 1 == to_x && from_y - 1 == to_y)
		return true;
	else if(from_x - 1 == to_x && from_y == to_y)
		return true;
	else if(from_x - 1 == to_x && from_y + 1 == to_y)
		return true;
	else if(from_x == to_x && from_y + 1 == to_y)
		return true;
	else if(from_x + 1 == to_x && from_y + 1 == to_y)
		return true;
	return false;
}

bool check_moveset(int from_x, int from_y, int to_x, int to_y)
{
	bool v;
	switch(board[from_x][from_y].piece) {
		case PAWN:
			v = check_pawn_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid pawn move" : "Invalid pawn move");
			return v;
		case KNIGHT:
			v = check_knight_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid knight move" : "Invalid knight move");
			return v;
		case BISHOP:
			v = check_bishop_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid bishop move" : "Invalid bishop move");
			return v;
		case ROOK:
			v = check_rook_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid rook move" : "Invalid rook move");
			return v;
		case QUEEN:
			v = check_bishop_moveset(from_x, from_y, to_x, to_y);
			v |= check_rook_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid queen move" : "Invalid queen move");
			return v;
		case KING:
			v = check_king_moveset(from_x, from_y, to_x, to_y);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, v ? "Valid king move" : "Invalid king move");
			return v;
		default:
			return false;
	}
}

bool is_in_check(int side)
{
	int x = side == WHITE ? w_x : b_x;
	int y = side == WHITE ? w_y : b_y;

	// Knight
	if(x + 1 >= 0 && x + 1 < 8 && y + 2 >= 0 && y + 2 < 8 && board[x + 1][y + 2].piece == KNIGHT
			&& board[x + 1][y + 2].side != side)
		return true;
	if(x + 2 >= 0 && x + 2 < 8 && y + 1 >= 0 && y + 1 < 8 && board[x + 2][y + 1].piece == KNIGHT
			&& board[x + 2][y + 1].side != side)
		return true;
	if(x + 2 >= 0 && x + 2 < 8 && y - 1 >= 0 && y - 1 < 8 && board[x + 2][y - 1].piece == KNIGHT
			&& board[x + 2][y - 1].side != side)
		return true;
	if(x + 1 >= 0 && x + 1 < 8 && y - 2 >= 0 && y - 2 < 8 && board[x + 1][y - 2].piece == KNIGHT
			&& board[x + 1][y - 2].side != side)
		return true;
	if(x - 1 >= 0 && x - 1 < 8 && y - 2 >= 0 && y - 2 < 8 && board[x - 1][y - 2].piece == KNIGHT
			&& board[x - 1][y - 2].side != side)
		return true;
	if(x - 2 >= 0 && x - 2 < 8 && y - 1 >= 0 && y - 1 < 8 && board[x - 2][y - 1].piece == KNIGHT
			&& board[x - 2][y - 1].side != side)
		return true;
	if(x - 2 >= 0 && x - 2 < 8 && y + 1 >= 0 && y + 1 < 8 && board[x - 2][y + 1].piece == KNIGHT
			&& board[x - 2][y + 1].side != side)
		return true;
	if(x - 1 >= 0 && x - 1 < 8 && y + 2 >= 0 && y + 2 < 8 && board[x - 1][y + 2].piece == KNIGHT
			&& board[x - 1][y + 2].side != side)
		return true;

	// Bishop and queen
	bool ne = true, se = true, sw = true, nw = true;
	for(int i = 1; i <= 7; i++) {
		if(x + i >= 0 && x + i < 8 && y + i >= 0 && y + i < 8 && ne && (board[x + i][y + i].piece == BISHOP
					|| board[x + i][y + i].piece == QUEEN) && board[x + i][y + i].side != side)
			return true;
		else if(x + i >= 0 && x + i < 8 && y + i >= 0 && y + i < 8 && ne && board[x + i][y + i].piece != EMPTY)
			ne = false;
		if(x + i >= 0 && x + i < 8 && y - i >= 0 && y - i < 8 && se && (board[x + i][y - i].piece == BISHOP
					|| board[x + i][y - i].piece == QUEEN) && board[x + i][y - i].side != side)
			return true;
		else if(x + i >= 0 && x + i < 8 && y - i >= 0 && y - i < 8 && se && board[x + i][y - i].piece != EMPTY)
			se = false;
		if(x - i >= 0 && x - i < 8 && y - i >= 0 && y - i < 8 && sw && (board[x - i][y - i].piece == BISHOP
					|| board[x - i][y - i].piece == QUEEN) && board[x - i][y - i].side != side)
			return true;
		else if(x - i >= 0 && x - i < 8 && y - i >= 0 && y - i < 8 && sw && board[x - i][y - i].piece != EMPTY)
			sw = false;
		if(x - i >= 0 && x - i < 8 && y + i >= 0 && y + i < 8 && nw && (board[x - i][y + i].piece == BISHOP
					|| board[x - i][y + i].piece == QUEEN) && board[x - i][y + i].side != side)
			return true;
		else if(x - i >= 0 && x - i < 8 && y + i >= 0 && y + i < 8 && nw && board[x - i][y + i].piece != EMPTY)
			nw = false;
	}

	// Rook and queen
	bool n = true, e = true, s = true, w = true;
	for(int i = 1; i <= 7; i++) {
		if(y + i >= 0 && y + i < 8 && n && (board[x][y + i].piece == ROOK || board[x][y + i].piece == QUEEN)
				&& board[x][y + i].side != side)
			return true;
		else if(y + i >= 0 && y + i < 8 && n && board[x][y + i].piece != EMPTY)
			n = false;
		if(x + i >= 0 && x + i < 8 && e && (board[x + i][y].piece == ROOK || board[x + i][y].piece == QUEEN)
				&& board[x + i][y].side != side)
			return true;
		else if(x + i >= 0 && x + i < 8 && e && board[x + i][y].piece != EMPTY)
			e = false;
		if(y - i >= 0 && y - i < 8 && s && (board[x][y - i].piece == ROOK || board[x][y - i].piece == QUEEN)
				&& board[x][y - i].side != side)
			return true;
		else if(y - i >= 0 && y - i < 8 && s && board[x][y - i].piece != EMPTY)
			s = false;
		if(x - i >= 0 && x - i < 8 && w && (board[x - i][y].piece == ROOK || board[x - i][y].piece == QUEEN)
				&& board[x - i][y].side != side)
			return true;
		else if(x - i >= 0 && x - i < 8 && w && board[x - i][y].piece != EMPTY)
			w = false;
	}
	return false;
}

void make_move(int from_x, int from_y, int to_x, int to_y)
{
	if(!check_moveset(from_x, from_y, to_x, to_y))
		return;
	int prev_x, prev_y;

	if(board[from_x][from_y].piece == KING && turn == WHITE) {
		prev_x = w_x;
		prev_y = w_y;
		w_x = to_x;
		w_y = to_y;
	} else if(board[from_x][from_y].piece == KING && turn == BLACK) {
		prev_x = b_x;
		prev_y = b_y;
		b_x = to_x;
		b_y = to_y;
	}

	Cell prev = board[to_x][to_y];
	board[to_x][to_y] = board[from_x][from_y];
	board[from_x][from_y].piece = EMPTY;

	if(is_in_check(turn)) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "This move puts your king in check!");
		board[from_x][from_y] = board[to_x][to_y];
		board[to_x][to_y] = prev;
		if(board[from_x][from_y].piece == KING && turn == WHITE) {
			w_x = prev_x;
			w_y = prev_y;
		} else if(board[from_x][from_y].piece == KING && turn == BLACK) {
			b_x = prev_x;
			b_y = prev_y;
		}
		return;
	}
	selected[0] = -1;
	selected[1] = -1;
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

// Event and input handling

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

	x_s = event.x / (SIZE / 8);
	y_s = (SIZE - event.y) / (SIZE / 8);
	if(selected[0] == -1 && (turn != board[x_s][y_s].side || board[x_s][y_s].piece == EMPTY))
		return;

	if(selected[0] == -1) {
		selected[0] = x_s;
		selected[1] = y_s;
	} else if(selected[0] != -1) {
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
