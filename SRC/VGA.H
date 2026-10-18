#ifndef VGA_H
#define VGA_H

#include <_defs.h>
// VGA GFX Mode
#define MODE_VGA_13H 0x13
// VGA Text Mode
#define MODE_VGA_3H  0x3

void _Cdecl _wait_vretrace(void);
void _Cdecl _put_pixel(int x, int y, char color);
void _Cdecl _init_mode(int mode);

#endif