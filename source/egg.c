#include "egg.h"

bool egg_check_s1_code(u32 pressed, u32 released)
{
  static int valid_nb=0;
  if(pressed >= 0x0004 && pressed <= 0x1000){
	
    switch (valid_nb){
    case 0:if(pressed & WPAD_BUTTON_UP)valid_nb++; else valid_nb=0;break;
    case 1:if(pressed & WPAD_BUTTON_DOWN)valid_nb++; else valid_nb=0;break;
    case 2:if(pressed & WPAD_BUTTON_LEFT)valid_nb++;else valid_nb=0;break;
    case 3:if(pressed & WPAD_BUTTON_RIGHT)valid_nb++; else valid_nb=0;break;
    case 4:if(pressed & WPAD_BUTTON_B)valid_nb++; else valid_nb=0;break;
    case 5:if(pressed & WPAD_BUTTON_A)valid_nb++; else valid_nb=0;break;
    case 6:if(pressed & WPAD_BUTTON_PLUS)valid_nb++; else valid_nb=0;break;
    }
  }
	
  if(valid_nb==7){
    valid_nb = 0;
    return true;
  }
  else return false;
}

bool egg_check_k_code(u32 pressed, u32 released)
{
  static int valid_nb=0;
  if(pressed >= 0x0004 && pressed <= 0x1000){
	
    switch (valid_nb){
    case 0:if(pressed & WPAD_BUTTON_UP)valid_nb++; else valid_nb=0;break;
    case 1:if(pressed & WPAD_BUTTON_UP)valid_nb++; else valid_nb=0;break;
    case 2:if(pressed & WPAD_BUTTON_DOWN)valid_nb++; else valid_nb=0;break;
    case 3:if(pressed & WPAD_BUTTON_DOWN)valid_nb++; else valid_nb=0;break;
    case 4:if(pressed & WPAD_BUTTON_LEFT)valid_nb++;else valid_nb=0;break;
    case 5:if(pressed & WPAD_BUTTON_RIGHT)valid_nb++; else valid_nb=0;break;
    case 6:if(pressed & WPAD_BUTTON_LEFT)valid_nb++;else valid_nb=0;break;
    case 7:if(pressed & WPAD_BUTTON_RIGHT)valid_nb++; else valid_nb=0;break;
    case 8:if(pressed & WPAD_BUTTON_B)valid_nb++; else valid_nb=0;break;
    case 9:if(pressed & WPAD_BUTTON_A)valid_nb++; else valid_nb=0;break;
    }
  }
	
  if(valid_nb==10){
    valid_nb = 0;
    return true;
  }
  else return false;
}

