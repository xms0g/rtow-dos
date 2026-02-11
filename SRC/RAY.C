#include "ray.h"

vec3 rayAt(const Ray* ray, double t) {
    vec3 scaledDir = v3MultiplyN(&ray->direction, t);
    return v3Add(&ray->origin, &scaledDir);
}