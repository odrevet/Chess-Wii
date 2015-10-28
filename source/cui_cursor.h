#ifndef CUI_CURSOR
#define CUI_CURSOR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>


typedef struct t_cui_cursor{
	GRRLIB_texImg *tex[2];		//0 is pointing, 1 is grabbing, 
	bool is_grabbing;
	int x;
	int y;
	int hotspot_x;
	int hotspot_y;
	int angle;
}cui_cursor;


void cui_cursor_display(cui_cursor* cursor);
void cui_cursor_update(cui_cursor* cursor, int x, int y, int angle);

#endif
