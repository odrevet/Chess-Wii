#include "cui_board.h"

void cui_board_init(cui_board* board)
{
  unsigned int i;
  unsigned int x=0, y=0;
    
  for (i = 0; i < CUI_TILE_NB; i++) {
    bool is_white = (x%2?false:true);
    if(y%2)is_white=!is_white;
    u32 tile_color = is_white? board->tile_color1 : board->tile_color2;

    cui_tile_set_color(&board->tile[i], tile_color, 3);
    if ((i & 7) == 7){
      y++;
      x=0;
    }else x++;
		
  }
}

void cui_board_read_core(cui_board *board, ENGINE engine)
{
  int i;

  switch(engine){
  case FIRSTCHESS:
    {	
      for (i=0; i < CUI_TILE_NB; i++) {
	if(FIRSTCHESS_piece[i] != EMPTY){
	  board->piece[i]=FIRSTCHESS_piece[i];
	  board->color[i]=FIRSTCHESS_color[i];
	}else{
	  board->piece[i] = CUI_EMPTY;				
	  board->color[i] = CUI_EMPTY;		
	}		
      }
    }
    break;
  case TSCP:
    {
      for (i=0; i < CUI_TILE_NB; i++) {
	board->piece[i]=piece[i];
	if(color[i] == 1){
	  board->color[i]=CUI_BLACK;
	}else if (color[i] == 6){
	  board->color[i]=CUI_EMPTY;
	}else if (color[i] == 0){
	  board->color[i]=CUI_WHITE;
	}
      }
    }
    break;
  }
}

void cui_board_display(cui_board *board, GRRLIB_texImg* tex_pieces, GRRLIB_texImg* tex_tile, GRRLIB_ttfFont* font, int cursor_x, int cursor_y){
  unsigned int i;
  unsigned int x=0, y=0;
    
  int offset_x = 140;
  int offset_y = 80;

  for(i=0;i<8;i++){
    char board_alpha[2] = {i + 'A'};
    GRRLIB_PrintfTTF (i*40+offset_x+20, offset_y + 8*40 + 16/2, font, board_alpha, 16, 0xFFFFFFFF);	
  }

  for (i = 0; i < CUI_TILE_NB; i++) {

    u32 tile_color;
    if(x == cursor_x && y == cursor_y){
      tile_color = 0x223388FF;
    }else tile_color = board->tile[i].color;
		
    GRRLIB_DrawImg(x*40+offset_x, y*40+offset_y, tex_tile, 0, 1, 1, tile_color);
        
    if ((i & 7) == 7){
      char board_num[2];
      snprintf(board_num, 2, "%d", 8-y);
      GRRLIB_PrintfTTF (offset_x - 20, y*40+offset_y + 16/2, font, board_num, 16, 0xFFFFFFFF);
			
      y++;
      x=0;
    }else{			
      x++;
    }
  }
   
  x=y=0;
  offset_x++;
  offset_y = 11;
  for (i = 0; i < CUI_TILE_NB; i++) {

    if (board->piece[i] != CUI_EMPTY){
      u32 piece_color = (board->color[i] == CUI_WHITE ? board->piece_color1 : board->piece_color2);
      GRRLIB_DrawPart(x*40+offset_x, y*40+offset_y, board->piece[i]*40, 0, 40, 100, tex_pieces, 0, 1, 1, piece_color);
    }
        
    if ((i & 7) == 7){
      y++;
      x=0;
    }else x++;
  }
}  

void cui_board_demo(cui_board *board, GRRLIB_texImg* tex_pieces, GRRLIB_texImg* tex_tile, GRRLIB_ttfFont* font){
  unsigned int i;
  unsigned int x=0, y=0;
    
  int offset_x = 140;
  int offset_y = 80;
    
  srand ( time(NULL) );

  for (i = 0; i < CUI_TILE_NB; i++) {
    u32 color1 = rand() % 0x0000FF88;
    u32 color2 = rand() % 0x0000FF88;
    
    bool is_white = (x%2?false:true);
    if(y%2)is_white=!is_white;
    u32 tile_color = is_white? color1 : color2;

    GRRLIB_DrawImg(x*40+offset_x, y*40+offset_y, tex_tile, 0, 1, 1, tile_color);
        
    if ((i & 7) == 7){
      char board_num[2];
      snprintf(board_num, 2, "%d", 8-y);
			
      y++;
      x=0;
    }else{			
      x++;
    }
  }
 
}  

void cui_board_update(cui_board *p_board, char coords[5])
{
  //p_board->piece[64];
  //p_board->color[64];
}

