#ifndef CUI_PIECE_H
#define CUI_PIECE_H

#include <stdbool.h>

typedef struct t_cui_piece{
	bool color;
	bool display;
	bool is_dragged;
}cui_piece;

/**
 * Display an animation of the piece moving througt tiles 
 * */
void cui_piece_move_to(cui_piece* piece, int from, int to);
#endif
