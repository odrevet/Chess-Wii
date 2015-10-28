#include "cui_game.h"

void cui_game_update_cursor(cui_cursor* cursor)
{
	WPAD_IR(0, &ir);
	WPAD_ScanPads();
	cui_cursor_update(cursor, ir.x, ir.y, ir.angle);
}



////////////////////////////////////////////////////
void GRRLIB_PrintfTTF2(int x, int y, GRRLIB_ttfFont *myFont, unsigned int fontSize, const u32 color, const char *string, ...) {
    if(myFont == NULL || string == NULL)
        return;

    char tmp[1024];
    va_list argp;
    va_start(argp, string);
    size_t length = vsprintf(tmp, string, argp) + 1;
    va_end(argp);

    wchar_t *utf32 = (wchar_t*)malloc(length * sizeof(wchar_t));
    if(utf32) {
        length = mbstowcs(utf32, tmp, length);
        if(length > 0) {
            utf32[length] = L'\0';
            GRRLIB_PrintfTTFW(x, y, myFont, utf32, fontSize, color);
        }
        free(utf32);
    }
}
