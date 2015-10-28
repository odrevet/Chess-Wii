#ifndef EGG_H
#define EGG_H

#include <stdbool.h>
#include <wiiuse/wpad.h>

bool egg_check_s1_code(u32 pressed, u32 released);
bool egg_check_k_code(u32 pressed, u32 released);

#endif
