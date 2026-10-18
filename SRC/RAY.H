#ifndef ray_H
#define ray_H

#include "vec3.h"

typedef struct ray {
    vec3 origin;
    vec3 direction;
} ray;

vec3 ray_at(const ray* ray, double t);

#endif