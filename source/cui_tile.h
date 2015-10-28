#ifndef CUI_TILE_H
#define CUI_TILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include <gccore.h>
#include <ogcsys.h>

typedef struct t_cui_tile{
  u32 color;
}cui_tile;

void cui_tile_set_color(cui_tile* tile, u32 color, float delay);
void cui_tile_set_color_back_to_normal(cui_tile* tile, float delay);

#endif
