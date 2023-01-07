#include <stdbool.h>
#include <SDL2/SDL.h>
#include "include/game.h"

bool turn;
int w_x, w_y;
int b_x, b_y;
Cell board[8][8];
int selected[2];

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
	// TODO promotion
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

	// Pawn
	if(side == WHITE) {
		if(board[x + 1][y + 1].side == BLACK && board[x + 1][y + 1].piece == PAWN)
			return true;
		if(board[x - 1][y + 1].side == BLACK && board[x - 1][y + 1].piece == PAWN)
			return true;
	} else {
		if(board[x + 1][y - 1].side == WHITE && board[x + 1][y - 1].piece == PAWN)
			return true;
		if(board[x - 1][y - 1].side == WHITE && board[x - 1][y - 1].piece == PAWN)
			return true;
	}

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
