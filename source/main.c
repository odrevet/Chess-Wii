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
#include "cui_engine.h"
#include "cui_board.h"
#include "cui_cursor.h"
#include "egg.h"
#include "sounds/bg_music.h"
#include "UI/button.h"
#include "cui_menu.h"

  

s8 HWButton = -1;

#define BURST_SIZE 5760
#define NUM_BUFFERS 18
static void *buffers[NUM_BUFFERS];
static u8 play_buffer;
static int queued;
static bool active;
static void aesnd_callback(AESNDPB *pb, u32 state)
{
  if (state == VOICE_STATE_STREAM) {
    if (queued > 0) {
      AESND_SetVoiceBuffer(pb, buffers[play_buffer], BURST_SIZE);
      play_buffer = (play_buffer + 1) % NUM_BUFFERS;
      queued -= BURST_SIZE;
    } else {
      AESND_SetVoiceStop(pb, true);
      active = false;
    }
  }
}

int main(int argc, char **argv) {

  GRRLIB_Init();	
	
  // Initialise the attached controllers
  WPAD_Init();
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	
  // Initialise the audio subsystem
  AESND_Init(NULL);
	
  MODPlay_Init(&play);
  MODPlay_SetMOD(&play,bg_music);
  MODPlay_SetVolume(&play,63,63);
  MODPlay_Start(&play);

  cui_cursor cursor;
  cursor.tex[0]=GRRLIB_LoadTexture(player1_point_png);
  cursor.tex[1]=GRRLIB_LoadTexture(player1_grab_png);
  cursor.is_grabbing=false; 
	
  cui_board board;
	
  ENGINE engine = FIRSTCHESS;
  if(engine == TSCP){
    int i;
    for (i=0; i < CUI_TILE_NB; i++) {
      board.piece[i]=init_piece[i];
      if(init_color[i] == 1){
	board.color[i]=CUI_BLACK;
      }else if (init_color[i] == 6){
	board.color[i]=CUI_EMPTY;
      }else if (init_color[i] == 0){
	board.color[i]=CUI_WHITE;
      }
    }
			
    init_hash();
    init_board();
    open_book();
    gen();
  }


  cui_board_read_core(&board, engine);

  GRRLIB_SetBackgroundColour(0x030, 0x30, 0x42, 0xFF);
	
  board.tile_color1 = 0xAAAAAAFF;
  board.tile_color2 = 0x444444FF;
  board.piece_color1 = 0xAAAAAAFF;
  board.piece_color2 = 0x444444FF;
	
  cui_board_init(&board);

  cui_game game;
  game.board = &board;
  game.engine = engine; 
  game.cursor = &cursor;
  game.is_mute = false;
  GRRLIB_texImg *tex_wallpaper = GRRLIB_LoadTexture(wall_jpg);
  game.tex_wallpaper = tex_wallpaper;
			
	
  voice = AESND_AllocateVoice(aesnd_callback);
  AESND_SetVoiceVolume(voice, 0xFF, 0xFF);
  AESND_SetVoiceStream(voice, true);

  cui_menu current_state = CUI_MENU_MAIN;
	 
  while(current_state != CUI_MENU_EXIT) {
    switch(current_state){
    case CUI_MENU_MAIN:	current_state=menu_main(&game);break;
    case CUI_MENU_PLAY:	current_state=menu_play(&game);break;
    case CUI_MENU_OPTION:current_state=menu_option(&game);break;
    case CUI_MENU_OPTION_GENERAL:current_state=menu_option_general(&game);break;
    case CUI_MENU_OPTION_GFX:current_state=menu_option_gfx(&game);break;
    case CUI_MENU_OPTION_SND:current_state=menu_option_snd(&game);break;
    case CUI_MENU_CREDITS:current_state=menu_credits(&game);break;
    case CUI_MENU_PAUSE:current_state=menu_pause(&game);break;
    case CUI_MENU_HOME:current_state=menu_home(&game);break;
    case CUI_MENU_EXIT:current_state=menu_exit(&game);break;
    default:break;
    }
  }
	
  GRRLIB_FreeTexture(tex_wallpaper);
  GRRLIB_FreeTexture(cursor.tex[0]);
  GRRLIB_FreeTexture(cursor.tex[1]);
  GRRLIB_Exit();
  return 0;
}


