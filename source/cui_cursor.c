#include "cui_cursor.h"

void cui_cursor_display(cui_cursor* cursor)
{
	int draw_index=0;
	if(cursor->is_grabbing){
		draw_index=1;
	}

	GRRLIB_DrawImg(cursor->x, cursor->y, cursor->tex[draw_index], 1, 1, 1, 0xFFFFFFFF);
}

void cui_cursor_update(cui_cursor* cursor, int x, int y, int angle)
{
	cursor->x = x;
	cursor->y = y;
	if(cursor->is_grabbing){
		cursor->hotspot_x = cursor->x + 12;
		cursor->hotspot_y = cursor->y + 12;
	}else{
		cursor->hotspot_x = cursor->x + 12;
		cursor->hotspot_y = cursor->y + 5;		
	}
	cursor->angle = angle;
}
