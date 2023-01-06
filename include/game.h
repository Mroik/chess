#pragma once

#include <stdbool.h>
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

extern bool turn;
extern int w_x, w_y;
extern int b_x, b_y;
extern Cell board[8][8];
extern int selected[2];

void setup_board();
bool check_pawn_moveset(int from_x, int from_y, int to_x, int to_y);
bool check_knight_moveset(int from_x, int from_y, int to_x, int to_y);
bool check_bishop_moveset(int from_x, int from_y, int to_x, int to_y);
bool check_rook_moveset(int from_x, int from_y, int to_x, int to_y);
bool check_king_moveset(int from_x, int from_y, int to_x, int to_y);
bool check_moveset(int from_x, int from_y, int to_x, int to_y);
bool is_in_check(int side);
void make_move(int from_x, int from_y, int to_x, int to_y);
