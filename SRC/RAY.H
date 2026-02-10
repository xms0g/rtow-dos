#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray {
    vec3 origin;
    vec3 direction;
};

#ifdef __cplusplus
extern "C" {
#endif
const vec3* rayOrigin(const struct Ray* ray);

const vec3* rayDirection(const struct Ray* ray);

vec3 rayAt(const struct Ray* ray, double t);

#ifdef __cplusplus
}
#endif

#endif