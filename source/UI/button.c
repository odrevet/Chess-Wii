#include "button.h"

void UI_InitBtn(UI_button* p_button, int x, int y, char* text)
{
  p_button->x=x;
  p_button->y=y;
  p_button->over=false;
  p_button->click=false;
  p_button->scale = 1;
  p_button->text_color=0X000000FF;
  if(text){
    p_button->text = (char*)strdup(text);
  }else p_button->text=NULL;
  p_button->on_in = UI_button_on_in_default;
  p_button->on_out = UI_button_on_out_default;
  p_button->on_click = UI_button_on_click_default;
  p_button->hotkey = 8;
}

void UI_DrawBtn(UI_button* p_button)
{
  GRRLIB_DrawImg(p_button->x, p_button->y, p_button->img, 0, p_button->scale, p_button->scale, 0xFFFFFFFF);	
  if(p_button->text){
    int txt_w = GRRLIB_WidthTTF (p_button->font, p_button->text, 16);
    GRRLIB_PrintfTTF (p_button->x + p_button->img->w / 2 - txt_w / 2, p_button->y +  p_button->img->h / 2 - 16 / 2, p_button->font, p_button->text, 16, p_button->text_color);
  }
}

void UI_UpdateBtn(UI_button* p_button, int cursor_x, int cursor_y, int hotkey)
{
	 
  if(GRRLIB_PtInRect(p_button->x, p_button->y, p_button->img->w * p_button->scale, p_button->img->h * p_button->scale, cursor_x, cursor_y)){
    if(hotkey == p_button->hotkey){
      p_button->click=true;
    }else p_button->click=false;
		
    if(!p_button->over){
      p_button->over = true;
      if(p_button->on_in){
	p_button->on_in(p_button);
      }
    }
  }
  else{
    if(p_button->over){
      p_button->over = false;
      if(p_button->on_out){
	p_button->on_out(p_button);
      }
    }
  }	
}

void* UI_button_on_in_default(void* arg){
  UI_button* p_button = (UI_button*)arg;
  p_button->text_color=0xFF0000FF;
  p_button->scale = 1.3;
  if(voice) AESND_PlayVoice(voice, VOICE_STEREO16, hit, hit_size, VOICE_FREQ48KHZ, 1, false);
  return NULL;
}

void* UI_button_on_out_default(void* arg){
  UI_button* p_button = (UI_button*)arg;
  p_button->text_color=0x000000FF;
  p_button->scale = 1;
  return NULL;
}

void* UI_button_on_click_default(void* arg){
  UI_button* p_button = (UI_button*)arg;
  p_button->text_color=0x00FF00FF;
  return NULL;
}
