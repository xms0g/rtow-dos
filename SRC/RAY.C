#include "ray.h"

vec3 ray_at(const ray* ray, double t) {
    vec3 scaled_dir = v3_mul_n(&ray->direction, t);
    return v3_add(&ray->origin, &scaled_dir);
}