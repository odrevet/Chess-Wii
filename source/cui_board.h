#ifndef CUI_BOARD_H
#define CUI_BOARD_H

#include <gccore.h>
#include <fat.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>

#include "cui_engine.h"
#include "cui_board.h"
#include "cui_tile.h"

#include "engines/firstchess/core.h"


#define CUI_TILE_NB 64
#define CUI_WHITE 0
#define CUI_BLACK 1
#define CUI_EMPTY 420

typedef struct t_cui_board{
	int piece[64];
	int color[64];
	u32 tile_color1;
	u32 tile_color2;
	u32 piece_color1;
	u32 piece_color2;
	cui_tile tile[64];
}cui_board;

void cui_board_init(cui_board* board);
/**
 * Read the internal variable of a given engine to set the cui board
 * Should be used once, for initialisation, and eventually to redraw 
 * in some special cases (board editor...)  
 * */
void cui_board_read_core(cui_board *board, ENGINE engine);
void cui_board_display(cui_board *board, GRRLIB_texImg* tex_pieces, GRRLIB_texImg* tex_tile, GRRLIB_ttfFont* font, int cursor_x, int cursor_y);
void cui_board_demo(cui_board *board, GRRLIB_texImg* tex_pieces, GRRLIB_texImg* tex_tile, GRRLIB_ttfFont* font);

/**
 * Update the cui board according the coords 
 * */
void cui_board_update(cui_board *board, char coords[5]);
#endif
