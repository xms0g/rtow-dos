#include "vec3.h"
#include "math.h"
#include "bool.h"

vec3 new_vec3(double x, double y, double z) {
    vec3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    return result;
}

vec3 v3_negate(const vec3* v) {
    vec3 result;
    
    result.x = -v->x;
    result.y = -v->y;
    result.z = -v->z;
    
    return result;
}

vec3 v3_add(const vec3* v0, const vec3* v1) {
    vec3 result;
    
    result.x = v0->x + v1->x;
    result.y = v0->y + v1->y;
    result.z = v0->z + v1->z;
    
    return result;
}

vec3 v3_add_n(vec3* v, double N) {
    vec3 result;

    result.x = v->x + N;
    result.y = v->y + N;
    result.z = v->z + N;

    return result;
}

vec3 v3_sub(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->x - v1->x;
    result.y = v0->y - v1->y;
    result.z = v0->z - v1->z;

    return result;
}

vec3 v3_mul(const vec3* v0, const vec3* v1) {
    vec3 result;
    result.x = v0->x * v1->x;
    result.y = v0->y * v1->y;
    result.z = v0->z * v1->z;
    return result;
}

vec3 v3_mul_n(const vec3* v, double N) {
    vec3 result;

    result.x = v->x * N;
    result.y = v->y * N;
    result.z = v->z * N;

    return result;
}

vec3 v3_divide(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->x / v1->x;
    result.y = v0->y / v1->y;
    result.z = v0->z / v1->z;

    return result;
}

vec3 v3_divide_n(const vec3* v, double N) {
    vec3 result;

    result.x = v->x / N;
    result.y = v->y / N;
    result.z = v->z / N;

    return result;
}

vec3 v3_normalize(vec3* v) {
    return v3_divide_n(v, v3_len(v));
}

double v3_len(const vec3* v) {
    return 1.0 / inv_sqrt(v3_len_squared(v));
}

double v3_len_squared(const vec3* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

double v3_dot(const vec3* v0, const vec3* v1) {
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;

}

bool v3_near_zero(const vec3* v) {
    // Return true if the vector is close to zero in all dimensions.
    return (bool)((fabs(v->x) < 1e-8) && (fabs(v->y) < 1e-8) && (fabs(v->z) < 1e-8));
}

vec3 v3_cross(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->y * v1->z - v0->z * v1->y;
    result.y = v0->z * v1->x - v0->x * v1->z;
    result.z = v0->x * v1->y - v0->y * v1->x;

    return result;
}

vec3 v3_unit(const vec3* v) {
    return v3_divide_n(v, v3_len(v));
}

vec3 v3_random(void) {
    vec3 result;

    result.x = randd();
    result.y = randd();
    result.z = randd();

    return result;
}

vec3 v3_random_range(double min, double max) {
    vec3 result;

    result.x = randd_range(min, max);
    result.y = randd_range(min, max);
    result.z = randd_range(min, max);

    return result;
}

vec3 v3_random_unit_vec(void) {
    for (;;) {
        vec3 p = v3_random_range(-1, 1);
        double lensq = v3_len_squared(&p);
        
        if (1e-160 < lensq && lensq <= 1) {
            return v3_unit(&p);
        }      
    }
}

vec3 v3_random_in_unit_disk(void) {
    for (;;) {
        vec3 p;
        p.x = randd_range(-1,1);
        p.y = randd_range(-1,1);
        p.z = 0;
        
        if (v3_len_squared(&p) < 1)
            return p;
    }
}

vec3 v3_random_on_hemisphere(const vec3* normal) {
    vec3 on_unit_sphere = v3_random_unit_vec();
    
    if (v3_dot(&on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    
    return v3_negate(&on_unit_sphere);
}

vec3 v3_reflect(const vec3* v, const vec3* n) {
    vec3 n_mul_dot = v3_mul_n(n, 2 * v3_dot(v, n));
    return v3_sub(v, &n_mul_dot);
}

vec3 v3_refract(vec3* uv, const vec3* n, double etaiOverEtat) {
    vec3 ray_out_parallel;
    double sqrted;
    vec3 negated = v3_negate(uv);
    double cos_theta = fmin(v3_dot(&negated, n), 1.0);
    vec3 n_mul_cos_theta = v3_mul_n(n, cos_theta);
    vec3 ray_out_perp = v3_add(uv, &n_mul_cos_theta);
    ray_out_perp = v3_mul_n(&ray_out_perp, etaiOverEtat);

    sqrted = 1.0 / inv_sqrt(fabs(1.0 - v3_len_squared(&ray_out_perp)));
    ray_out_parallel = v3_mul_n(n, -sqrted);
    
    return v3_add(&ray_out_perp, &ray_out_parallel);
}

vec3 v3_lerp(const vec3* v0, const vec3* v1, double t) {
    vec3 v0_mult = v3_mul_n(v0, 1 - t);
    vec3 v1_mult = v3_mul_n(v1, t);

    return v3_add(&v0_mult, &v1_mult);
}