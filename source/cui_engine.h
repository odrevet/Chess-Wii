#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "engines/firstchess/core.h"

#include "engines/TSCP/defs.h"
#include "engines/TSCP/data.h"
#include "engines/TSCP/protos.h"

typedef enum E_ENGINE{
  FIRSTCHESS,
  TSCP, 
  GNUCHESS4
}ENGINE;

/**
 * glu goes here
 * */
bool calculate_coords(char coords[5], char newcoord[5], ENGINE engine);


#endif
