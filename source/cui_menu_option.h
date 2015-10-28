#ifndef CUI_MENU_OPTION_H
#define CUI_MENU_OPTION_H

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
#include "sounds/bg_music_b.h"
#include "sounds/bg_music_c.h"
#include "sounds/bg_music_d.h"
#include "sounds/warp.h"
#include "UI/button.h"
#include "cui_menu.h"

enum e_cui_menu menu_option(cui_game* p_game);
enum e_cui_menu menu_option_general(cui_game* p_game);
enum e_cui_menu menu_option_gfx(cui_game* p_game);
enum e_cui_menu menu_option_snd(cui_game* p_game);

#endif
