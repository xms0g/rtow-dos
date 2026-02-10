#ifndef VEC_H
#define VEC_H

#include "bool.h"
 
typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

#ifdef __cplusplus
extern "C" {
#endif

vec3 v3Negate(const vec3* v);

vec3 v3Add(const vec3* v0, const vec3* v1);

vec3 v3Subtract(const vec3* v0, const vec3* v1);

vec3 v3Multiply(const vec3* v0, const vec3* v1);

vec3 v3MultiplyN(const vec3* v, double n);

vec3 v3Divide(const vec3* v0, const vec3* v1);

vec3 v3DivideN(const vec3* v, double n);

void normalize(vec3* v);

double invLen(const vec3* v);

double lenSquared(const vec3* v);

double dot(const vec3* v0, const vec3* v1);

int nearZero(const vec3* v);

vec3 cross(const vec3* v0, const vec3* v1);

vec3 unit(const vec3* v);

vec3 v3Random();

vec3 v3RandomMinMax(double min, double max);

vec3 v3RandomUnitVec();

vec3 randomInUnitDisk();

vec3 randomOnHemisphere(const vec3* normal);

vec3 reflect(const vec3* v, const vec3* n);

vec3 refract(vec3* uv, const vec3* n, double etaiOverEtat);

#ifdef __cplusplus
}
#endif
#endif