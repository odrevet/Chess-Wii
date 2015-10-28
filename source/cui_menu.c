#include "cui_menu.h"


cui_menu menu_main(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);  
	GRRLIB_texImg *tex_pieces = GRRLIB_LoadTexture(chess_classic_png);
	GRRLIB_texImg *tex_tile = GRRLIB_LoadTexture(keyboard_key_png);
	GRRLIB_texImg *tex_btn = GRRLIB_LoadTexture(button_png);
	
	UI_button button_newgame;
	UI_InitBtn(&button_newgame, 280, 120, "New game");
	button_newgame.img = tex_btn;
	button_newgame.font = font;
	
	UI_button button_options;
	UI_InitBtn(&button_options, 280, 200, "Options");
	button_options.img = tex_btn;
	button_options.font = font;
	
	UI_button button_credit;
	UI_InitBtn(&button_credit, 280, 280, "credit");
	button_credit.img = tex_btn;
	button_credit.font = font;
	
	while(!done){		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		GRRLIB_PrintfTTF (5, 5, font, "ChessUI", 96, 0x424242FF);
		UI_UpdateBtn(&button_newgame, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_options, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_credit, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		if(button_newgame.click){
			done=true;
			next_state=CUI_MENU_PLAY;
		}

		if(button_options.click){
			done=true;
			next_state=CUI_MENU_OPTION;
		}
		
		if(button_credit.click){
			done=true;
			next_state=CUI_MENU_CREDITS;
		}
		
		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}

		
		if ( pressed & WPAD_BUTTON_A ){

			if(voice) AESND_PlayVoice(voice, VOICE_STEREO16, button_click_pcm, button_click_pcm_size, VOICE_FREQ48KHZ, 1, false);
		}
		
		cui_board_demo(p_game->board, tex_pieces, tex_tile, font);
		UI_DrawBtn(&button_newgame);
		UI_DrawBtn(&button_options);
		UI_DrawBtn(&button_credit);
		
		cui_cursor_display(p_game->cursor);
		
		GRRLIB_Render();
	}
	
	GRRLIB_FreeTTF(font);
	GRRLIB_FreeTexture(tex_btn);
	free(button_newgame.text);
	free(button_options.text);
	free(button_credit.text);
	return next_state;
}

cui_menu menu_play(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	
	GRRLIB_texImg *tex_pieces = GRRLIB_LoadTexture(chess_classic_png);
	GRRLIB_texImg *tex_tile = GRRLIB_LoadTexture(keyboard_key_png);	
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size); 
	bool is_debug = false;

	char coords[5];
	char calculated_coords[5];
	
	strcpy(coords, "d2d20");
	strcpy(calculated_coords, "d2d20");
	
	while(!done){
		WPAD_IR(0, &ir);
		WPAD_ScanPads();
		
		u32 pressed = WPAD_ButtonsDown(0);
		u32 released = WPAD_ButtonsUp(0);
		
		if(egg_check_s1_code(pressed, released)){
			is_debug = true;
			p_game->board->tile_color1 = 0xc66300FF;
			p_game->board->tile_color2 = 0x632100FF;
			p_game->board->piece_color1 = 0x4242CCFF;
			p_game->board->piece_color2 = 0x444411FF;
			cui_board_init(p_game->board);
			if(voice) AESND_PlayVoice(voice, VOICE_STEREO8, warp, warp_size, VOICE_FREQ32KHZ, 1, false);
		}

		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}
		
		if ( pressed & WPAD_BUTTON_MINUS){
			switch(menu_option(p_game)){
				case CUI_MENU_OPTION_GFX:menu_option_gfx(p_game);break;
				case CUI_MENU_OPTION_SND:menu_option_snd(p_game);break;
				default:break;
			}
		}
		
		int index_offset_x = (p_game->cursor->hotspot_x - 40 / 2)/ 40 - 140 / 40;
		int index_offset_y = p_game->cursor->hotspot_y / 40 - 80 / 40;
		
		if (pressed & WPAD_BUTTON_B) {
			p_game->cursor->is_grabbing = true;
			
			if(index_offset_x >= 0 && index_offset_x < 8 && index_offset_y >= 0 && index_offset_y < 8){
				coords[0] = 'a' + index_offset_x;
				coords[1] = '8' - index_offset_y;
			}
				
		}
		else if(released & WPAD_BUTTON_B){
			p_game->cursor->is_grabbing = false;

			if(index_offset_x >= 0 && index_offset_x < 8 && index_offset_y >= 0 && index_offset_y < 8){
				coords[2] = 'a' + index_offset_x;
				coords[3] = '8' - index_offset_y;
				coords[4] = 0;

				if(voice) AESND_PlayVoice(voice, VOICE_STEREO16, move_pcm, move_pcm_size, VOICE_FREQ48KHZ, 1, false);
				
				if(calculate_coords(coords, calculated_coords, p_game->engine)){
					cui_board_read_core(p_game->board, p_game->engine);
				}
				
			}
		}
		
		cui_cursor_update(p_game->cursor, ir.x, ir.y, ir.angle);
		
		
		if(p_game->tex_wallpaper)GRRLIB_DrawImg(0, -50, p_game->tex_wallpaper, 1, 1, 1, 0XFFFFFFFF);
		
		cui_board_display(p_game->board, tex_pieces, tex_tile, font, index_offset_x, index_offset_y);
		cui_cursor_display(p_game->cursor);
		
		if(is_debug){
			GRRLIB_PrintfTTF (20, 00, font, coords, 16, 0x424242FF);
			GRRLIB_PrintfTTF2 (20, 20, font, 16, 0xFFFFFFFF, "x:%.2f y:%.2f", ir.x, ir.y);
			GRRLIB_PrintfTTF2 (20, 40, font, 16, 0xFFFFFFFF, "x:%.2f y:%.2f", ir.x/40, ir.y/40);
			GRRLIB_PrintfTTF2 (20, 60, font, 16, 0xFFFFFFFF, "x:%d y:%d", index_offset_x, index_offset_y);
			GRRLIB_PrintfTTF (20, 80, font, p_game->cursor->is_grabbing?"GRAB":"POINT", 16, 0xFFFFFFFF);
			
			GRRLIB_Rectangle(p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, 4, 4, 0xFF4242FF, 1);
		}
	
		GRRLIB_Render();
	}
	
	GRRLIB_FreeTexture(tex_pieces);
	GRRLIB_FreeTexture(tex_tile);
	GRRLIB_FreeTTF(font);
		
	return next_state;
}


cui_menu menu_credits(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);  
		
	UI_button button_return;
	UI_InitBtn(&button_return, 400, 400, "Return");
	button_return.img = GRRLIB_LoadTexture(button_png);
	button_return.font = font;
	
	while(!done){		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		UI_UpdateBtn(&button_return, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		
		GRRLIB_PrintfTTF2 (20, 60, font, 20, 0xFFFFFFFF, "%s", "ChessUi -- a chess interface for Wii --");
		GRRLIB_PrintfTTF2 (20, 80, font, 20, 0xFFFFFFFF, "%s", "Presented & programmed by TheDrev");
		GRRLIB_PrintfTTF2 (20, 100, font, 20, 0xFFFFFFFF, "%s", "Using the FirstChess engine by Pham Hong Nguyen ");
		GRRLIB_PrintfTTF2 (20, 120, font, 20, 0xFFFFFFFF, "%s", "and piece graphismes by Wapcaplet");
		GRRLIB_PrintfTTF2 (20, 140, font, 20, 0xFFFFFFFF, "%s", "and also the GRRLIB graphic library");
		GRRLIB_PrintfTTF2 (20, 160, font, 20, 0xFFFFFFFF, "%s", "see http://wiibrew.org/wiki/Chessui for more details");
		
		if(button_return.click){
			done=true;
			next_state=CUI_MENU_MAIN;
		}
		
		if ( pressed & WPAD_BUTTON_HOME ){
			if(!p_game->is_mute)AESND_Pause(true);
			if(menu_home(p_game) == CUI_MENU_EXIT){done=true;next_state=CUI_MENU_EXIT;}	
			if(!p_game->is_mute)AESND_Pause(false);
		}

		UI_DrawBtn(&button_return);
		
		cui_cursor_display(p_game->cursor);
		
		GRRLIB_Render();
	}
	
	GRRLIB_FreeTTF(font);
	free(button_return.text);
	GRRLIB_FreeTexture(button_return.img);
	
	return next_state;
}

cui_menu menu_pause(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	
	return next_state;
}

cui_menu menu_home(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_MAIN;
	bool done=false;
	GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size); 
	
	UI_button button_loader;
	UI_InitBtn(&button_loader, 200, 200, "Loader");
	button_loader.img = GRRLIB_LoadTexture(button_png);
	button_loader.font = font;
	
	UI_button button_menu;
	UI_InitBtn(&button_menu, 400, 200, "Wii menu");
	button_menu.img = GRRLIB_LoadTexture(button_png);
	button_menu.font = font;
	

	while(!done)
	{		
		GRRLIB_FillScreen(0x000000FF);
		GRRLIB_PrintfTTF (15, 0, font, "HOME Menu", 96, 0xFFFFFFFF);
		GRRLIB_Line(0, 100, 640, 100, 0xFFFFFFFF);
		UI_DrawBtn(&button_loader);
		UI_DrawBtn(&button_menu);
		cui_cursor_display(p_game->cursor);
		
		cui_game_update_cursor(p_game->cursor);
		u32 pressed = WPAD_ButtonsDown(0);
		
		UI_UpdateBtn(&button_loader, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		UI_UpdateBtn(&button_menu, p_game->cursor->hotspot_x, p_game->cursor->hotspot_y, pressed & WPAD_BUTTON_A);
		
		if ( pressed & WPAD_BUTTON_HOME ){
			done = true;
		}
		
		if(button_loader.click){
			done=true;
			next_state=CUI_MENU_EXIT;
		}
		
		if(button_menu.click){
			SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
		}
		
		GRRLIB_Render();
		
	}
	
	GRRLIB_FreeTexture(button_loader.img);
	GRRLIB_FreeTexture(button_menu.img);
	free(button_loader.text);
	free(button_menu.text);
	
	return next_state;
}

cui_menu menu_exit(cui_game* p_game)
{
	cui_menu next_state = CUI_MENU_EXIT;
	exit(0);
	
	return next_state;
}

