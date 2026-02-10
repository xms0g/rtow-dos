#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

typedef vec3 color;

#ifdef __cplusplus
extern "C" {
#endif
unsigned char rgb2vga(int r, int g, int b);

unsigned char pixel2vga(const color* pixel);

double linear2gamma(double linearComponent);

#ifdef __cplusplus
}
#endif

#endif