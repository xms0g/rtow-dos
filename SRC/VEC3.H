#ifndef VEC_H
#define VEC_H

#include "bool.h"
 
typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

vec3 new_vec3(double x, double y, double z);

vec3 v3_negate(const vec3* v);

vec3 v3_add(const vec3* v0, const vec3* v1);

vec3 v3_add_n(vec3* v, double N);

vec3 v3_sub(const vec3* v0, const vec3* v1);

vec3 v3_mul(const vec3* v0, const vec3* v1);

vec3 v3_mul_n(const vec3* v, double n);

vec3 v3_divide(const vec3* v0, const vec3* v1);

vec3 v3_divide_n(const vec3* v, double n);

vec3 v3_normalize(vec3* v);

double v3_len(const vec3* v);

double v3_len_squared(const vec3* v);

double v3_dot(const vec3* v0, const vec3* v1);

bool v3_near_zero(const vec3* v);

vec3 v3_cross(const vec3* v0, const vec3* v1);

vec3 v3_unit(const vec3* v);

vec3 v3_random(void);

vec3 v3_random_range(double min, double max);

vec3 v3_random_unit_vec(void);

vec3 v3_random_in_unit_disk(void);

vec3 v3_random_on_hemisphere(const vec3* normal);

vec3 v3_reflect(const vec3* v, const vec3* n);

vec3 v3_refract(vec3* uv, const vec3* n, double etaiOverEtat);

vec3 v3_lerp(const vec3* v0, const vec3* v1, double t);

#endif