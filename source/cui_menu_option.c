#include "cui_menu_option.h"


enum e_cui_menu menu_option(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);  
/*
	UI_button button_general;
	UI_InitBtn(&button_general, 280, 120, "General");
	button_general.img = GRRLIB_LoadTexture(button_png);
	button_general.font = font;
*/	
	UI_button button_graphics;
	UI_InitBtn(&button_graphics, 280, 200, "Graphics");
	button_graphics.img = GRRLIB_LoadTexture(button_png);
	button_graphics.font = font;
	
	UI_button button_sounds;
	UI_InitBtn(&button_sounds, 280, 280, "Sounds");
	button_sounds.img = GRRLIB_LoadTexture(button_png);
	button_sounds.font = font;
	
	UI_button button_return;
	UI_InitBtn(&button_return, 400, 400, "Return");
	button_return.img = GRRLIB_LoadTexture(button_png);
	button_return.font = font;
	
	while(!done){		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		//UI_UpdateBtn(&button_general, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_graphics, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_sounds, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_return, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		/*if(button_general.click){
			done=true;
			next_state=CUI_MENU_OPTION_GENERAL;
		}*/

		if(button_graphics.click){
			done=true;
			next_state=CUI_MENU_OPTION_GFX;
		}
		
		if(button_sounds.click){
			done=true;
			next_state=CUI_MENU_OPTION_SND;
		}
		
		if(button_return.click){
			done=true;
			next_state=CUI_MENU_MAIN;
		}
		
		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}

		
		//UI_DrawBtn(&button_general);
		UI_DrawBtn(&button_graphics);
		UI_DrawBtn(&button_sounds);
		UI_DrawBtn(&button_return);
		
		cui_cursor_display(p_game->cursor);
		
		GRRLIB_Render();
	}
	
	GRRLIB_FreeTexture(button_graphics.img);free(button_graphics.text);
	GRRLIB_FreeTexture(button_sounds.img);free(button_sounds.text);
	GRRLIB_FreeTexture(button_return.img);free(button_return.text);
	
	GRRLIB_FreeTTF(font);
	
	//GRRLIB_F(font);
	
	return next_state;
}


enum e_cui_menu menu_option_gfx(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	u32 option_tile_color_1 = p_game->board->tile_color1;
	u32 option_tile_color_2 = p_game->board->tile_color2;
	u32 option_piece_color_1 = p_game->board->piece_color1;
	u32 option_piece_color_2 = p_game->board->piece_color2;
	
	GRRLIB_texImg *tex_tile = GRRLIB_LoadTexture(keyboard_key_png);
	GRRLIB_texImg *tex_pieces = GRRLIB_LoadTexture(chess_classic_png);
	GRRLIB_texImg *tex_up = GRRLIB_LoadTexture(scrollbar_arrowup_png);
	GRRLIB_texImg *tex_down = GRRLIB_LoadTexture(scrollbar_arrowdown_png);
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);  
	
	int i,j;
	UI_button button_color[4][8];		//tile 1, tile 2, piece 1, piece 2 | R-+, G-+, B-+, A-+,
	for(i=0;i<4;i++){
		for(j=0;j<8;j++){
			UI_InitBtn(&button_color[i][j], 120 + j*64, 64 + i*80, NULL);
			button_color[i][j].img = j%2==0?tex_down:tex_up;
		}
	}
		
	UI_button button_return;
	UI_InitBtn(&button_return, 500, 400, "apply");
	button_return.img = GRRLIB_LoadTexture(button_png);
	button_return.font = font;
	
	UI_button button_cancel;
	UI_InitBtn(&button_cancel, 350, 400, "cancel");
	button_cancel.img = GRRLIB_LoadTexture(button_png);
	button_cancel.font = font;
	
	while(!done){		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		
		u32* color_to_modify=NULL;
		u32 mask;
		for(i=0;i<4;i++){
			for(j=0;j<8;j++){
				UI_UpdateBtn(&button_color[i][j], p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
				if(button_color[i][j].click){
					switch(i){
						case 0:color_to_modify = &option_tile_color_1;break;
						case 1:color_to_modify = &option_tile_color_2;break;
						case 2:color_to_modify = &option_piece_color_1;break;
						case 3:color_to_modify = &option_piece_color_2;break;
					}
					if(j==0 || j==1)mask=0x06000000;
					if(j==2 || j==3)mask=0x00060000;
					if(j==4 || j==5)mask=0x00000600;
					if(j==6 || j==7)mask=0x00000006;
					*color_to_modify+=j%2==0?-mask:mask;
				}
			}
		}


		
		UI_UpdateBtn(&button_cancel, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_return, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		if(button_return.click){
			if((option_tile_color_1 != p_game->board->tile_color1) || (option_tile_color_2 != p_game->board->tile_color2)){
				p_game->board->tile_color1 = option_tile_color_1;
				p_game->board->tile_color2 = option_tile_color_2;
				cui_board_init(p_game->board);
			}
			
			if(option_tile_color_1 != p_game->board->piece_color1)p_game->board->piece_color1=option_piece_color_1;
			if(option_tile_color_2 != p_game->board->piece_color2)p_game->board->piece_color2=option_piece_color_2;
					
			done=true;
			next_state=CUI_MENU_MAIN;
		}
		
		if(button_cancel.click){
			done=true;
			next_state=CUI_MENU_MAIN;
		}
		
		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}

		GRRLIB_DrawImg(380, 80+140, tex_tile, 0, 2, 2, option_tile_color_1);
		GRRLIB_DrawImg(380, 180+140, tex_tile, 0, 2, 2, option_tile_color_2);
		GRRLIB_DrawImg(380+80, 80+140, tex_tile, 0, 2, 2, option_tile_color_2);
		GRRLIB_DrawImg(380+80, 180+140, tex_tile, 0, 2, 2, option_tile_color_1);
		GRRLIB_DrawImg(380+160, 80+140, tex_tile, 0, 2, 2, option_tile_color_1);
		GRRLIB_DrawImg(380+160, 180+140, tex_tile, 0, 2, 2, option_tile_color_2);
		GRRLIB_DrawImg(380, 80, tex_pieces, 0, 2, 2, option_piece_color_2);
		GRRLIB_DrawImg(380, 180, tex_pieces, 0, 2, 2, option_piece_color_1);
		
		
		GRRLIB_PrintfTTF (145, 0, font, "RED              GREEN              BLUE             ALPHA", 20, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (15, 40, font, "Tiles color #1", 20, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (15, 40+80, font, "Tiles color #2", 20, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (15, 40+80*2, font, "Pieces color #1", 20, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (15, 40+80*3, font, "Pieces color #2", 20, 0xFFFFFFFF);

		for(i=0;i<4;i++){
			for(j=0;j<8;j++){
				UI_DrawBtn(&button_color[i][j]);
			}
		}


		UI_DrawBtn(&button_return);
		UI_DrawBtn(&button_cancel);
		
		GRRLIB_Line(235, 25, 235, 350, 0xFFFFFFFF);
		GRRLIB_Line(235+64*2, 25, 235+64*2, 350, 0xFFFFFFFF);
		GRRLIB_Line(235+64*4, 25, 235+64*4, 350, 0xFFFFFFFF);
		GRRLIB_Line(235+64*6, 25, 235+64*6, 350, 0xFFFFFFFF);
		
		cui_cursor_display(p_game->cursor);
		
		GRRLIB_Render();
	}
	
	GRRLIB_FreeTexture(tex_tile);
	GRRLIB_FreeTexture(tex_pieces);
	GRRLIB_FreeTexture(tex_up);
	GRRLIB_FreeTexture(tex_down);
	GRRLIB_FreeTTF(font); 
	
	free(button_return.text);
	free(button_cancel.text);
	
	return next_state;	
}

enum e_cui_menu menu_option_snd(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);  
	GRRLIB_texImg *tex_btn = GRRLIB_LoadTexture(button_png);
	
	UI_button button_sounds;
	UI_InitBtn(&button_sounds, 200, 280, "Mute");
	button_sounds.img = tex_btn;
	button_sounds.font = font;
	
	UI_button button_return;
	UI_InitBtn(&button_return, 400, 400, "Return");
	button_return.img = tex_btn;
	button_return.font = font;
	
	
	
	int i;
	UI_button button_type[4];
	for(i=0;i<4;i++){
		char text[7];
		sprintf(text, "TYPE %c", 'A'+i);
		UI_InitBtn(&button_type[i], 50+150*i, 100, text);
		button_type[i].img = tex_btn;
		button_type[i].font = font;	
	}

	while(!done){		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		for(i=0;i<4;i++){
			UI_UpdateBtn(&button_type[i], p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		}
		
		UI_UpdateBtn(&button_sounds, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_return, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		for(i=0;i<4;i++){
			if(button_type[i].click){
				switch(i){
					case 0:MODPlay_SetMOD(&play,bg_music);MODPlay_Start(&play);break;
					case 1:MODPlay_SetMOD(&play,bg_music_b);MODPlay_Start(&play);break;
					case 2:MODPlay_SetMOD(&play,bg_music_c);MODPlay_Start(&play);break;
					case 3:MODPlay_SetMOD(&play,bg_music_d);MODPlay_Start(&play);break;
				}
			}
		}
		
		if(button_sounds.click){
			p_game->is_mute = !p_game->is_mute;
			AESND_Pause(p_game->is_mute);
		}
		
		if(button_return.click){
			done=true;
			next_state=CUI_MENU_MAIN;
		}
		
		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}

		for(i=0;i<4;i++){
			UI_DrawBtn(&button_type[i]);
		}
	
		UI_DrawBtn(&button_sounds);
		UI_DrawBtn(&button_return);
		
		cui_cursor_display(p_game->cursor);
		
		GRRLIB_Render();
	}
	

	free(button_sounds.text);
	free(button_return.text);
	free(button_type[0].text);
	free(button_type[1].text);
	free(button_type[2].text);
	free(button_type[3].text);
	GRRLIB_FreeTexture(tex_btn);
	GRRLIB_FreeTTF(font); 
	
	return next_state;
}

enum e_cui_menu menu_option_general(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	
	return next_state;
}
