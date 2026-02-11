#ifndef MATH_H
#define MATH_H

#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

#define fabs(n) ((n) < 0.0 ? -(n) : (n))

#define fmin(n0, n1) ((n0 < n1) ? n0 : n1)

#define fmax(n0, n1) ((n0 > n1) ? n0 : n1)

#define degree2radian(angle) (angle * M_PI / 180.0)

#define clamp(v, lo, hi) (v < lo ? lo : (v > hi ? hi : v))

#define randd() (rand() / (RAND_MAX + 1.0))

#define randdRange(min, max) (min + (max - min) * randd())

double invSqrt(double n);

#endif