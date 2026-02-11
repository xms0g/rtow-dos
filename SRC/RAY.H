#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct Ray {
    vec3 origin;
    vec3 direction;
} Ray;

vec3 rayAt(const Ray* ray, double t);

#endif