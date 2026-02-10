#include "ray.h"

const vec3* rayOrigin(const struct Ray* ray) {
    return &ray->origin;
}

const vec3* rayDirection(const struct Ray* ray) {
    return &ray->direction;
}

vec3 rayAt(const struct Ray* ray, double t) {
    vec3 scaledDir = v3MultiplyN(rayDirection(ray), t);
    return v3Add(rayOrigin(ray), &scaledDir);
}