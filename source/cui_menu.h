#ifndef CUI_MENU_H
#define CUI_MENU_H

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

#include "filelist.h"
#include "cui_game.h"
#include "cui_engine.h"
#include "cui_board.h"
#include "cui_cursor.h"
#include "egg.h"
#include "sounds/bg_music.h"
#include "sounds/warp.h"
#include "UI/button.h"
#include "cui_menu_option.h"

typedef enum e_cui_menu{
  CUI_MENU_MAIN,
  CUI_MENU_PLAY,
  CUI_MENU_OPTION,
  CUI_MENU_OPTION_GENERAL,
  CUI_MENU_OPTION_GFX,
  CUI_MENU_OPTION_SND,
  CUI_MENU_CREDITS,
  CUI_MENU_PAUSE,
  CUI_MENU_HOME,
  CUI_MENU_EXIT
}cui_menu; 

cui_menu menu_main(cui_game* p_game);
cui_menu menu_play(cui_game* p_game);
cui_menu menu_pause(cui_game* p_game);
cui_menu menu_home(cui_game* p_game);
cui_menu menu_exit(cui_game* p_game);
cui_menu menu_credits(cui_game* p_game);

#endif
