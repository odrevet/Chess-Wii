#ifndef CUI_GAME_H
#define CUI_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include <gccore.h>
#include <ogcsys.h>
#include <fat.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>
#include <gcmodplay.h>
#include <aesndlib.h> 


#include "cui_board.h"
#include "cui_cursor.h"

ir_t ir;
AESNDPB *voice;
MODPlay play;

typedef struct t_cui_game{
  ENGINE engine;
  cui_board *board;
  cui_cursor *cursor;
  GRRLIB_texImg *tex_wallpaper;
	
  //options
  bool is_mute;
  int volume_music;
  int volume_effects;
  u32 piece_color1;
  u32 piece_color2;
}cui_game;

void cui_game_update_cursor(cui_cursor* cursor);

/////////////////////////////////////////////////////////
void GRRLIB_PrintfTTF2(int x, int y, GRRLIB_ttfFont *myFont, unsigned int fontSize, const u32 color, const char *string, ...);  

#endif
