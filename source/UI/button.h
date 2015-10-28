#ifndef UI_BUTTON
#define UI_BUTTON

#include <stdbool.h>

#include <gccore.h>
#include <ogcsys.h>
#include <fat.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>
#include <grrlib/GRRLIB__inline.h>
#include <gcmodplay.h>
#include <aesndlib.h> 

#include "../cui_game.h"
#include "../sounds/hit.h"

typedef struct t_UI_button{
  char* 			text;
  u32				text_color;
  GRRLIB_texImg* 	img;
  GRRLIB_ttfFont* font;
  int x;
  int y;
  bool over;
  bool click;
  u32 hotkey;
  float scale;
  void*(*on_in)(void*);
  void*(*on_out)(void*);
  void*(*on_click)(void*);
}UI_button;

void UI_InitBtn(UI_button* p_button, int x, int y, char* text);
void UI_DrawBtn(UI_button* p_button);
void UI_UpdateBtn(UI_button* p_button, int cursor_x, int cursor_y, int hotkey);

//Callback functions by default
void* UI_button_on_in_default(void* arg);
void* UI_button_on_out_default(void* arg);
void* UI_button_on_click_default(void* arg);

#endif
