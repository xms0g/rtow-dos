#ifndef VEC_H
#define VEC_H

#include "bool.h"
 
typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

vec3 newVec3(double x, double y, double z);

vec3 v3Negate(const vec3* v);

vec3 v3Add(const vec3* v0, const vec3* v1);

vec3 v3AddN(vec3* v, double N);

vec3 v3Subtract(const vec3* v0, const vec3* v1);

vec3 v3Multiply(const vec3* v0, const vec3* v1);

vec3 v3MultiplyN(const vec3* v, double n);

vec3 v3Divide(const vec3* v0, const vec3* v1);

vec3 v3DivideN(const vec3* v, double n);

vec3 v3Normalize(vec3* v);

double v3Len(const vec3* v);

double v3LenSquared(const vec3* v);

double v3Dot(const vec3* v0, const vec3* v1);

bool v3NearZero(const vec3* v);

vec3 v3Cross(const vec3* v0, const vec3* v1);

vec3 v3Unit(const vec3* v);

vec3 v3Random();

vec3 v3RandomRange(double min, double max);

vec3 v3RandomUnitVec();

vec3 v3RandomInUnitDisk();

vec3 v3RandomOnHemisphere(const vec3* normal);

vec3 v3Reflect(const vec3* v, const vec3* n);

vec3 v3Refract(vec3* uv, const vec3* n, double etaiOverEtat);

#endif