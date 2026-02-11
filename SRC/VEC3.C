#include "vec3.h"
#include "math.h"
#include "bool.h"

vec3 v3Negate(const vec3* v) {
    vec3 result;
    
    result.x = -v->x;
    result.y = -v->y;
    result.z = -v->z;
    
    return result;
}

vec3 v3Add(const vec3* v0, const vec3* v1) {
    vec3 result;
    
    result.x = v0->x + v1->x;
    result.y = v0->y + v1->y;
    result.z = v0->z + v1->z;
    
    return result;
}

void v3AddN(vec3* v, double N) {
    v->x += N;
    v->y += N;
    v->z += N;
}

vec3 v3Subtract(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->x - v1->x;
    result.y = v0->y - v1->y;
    result.z = v0->z - v1->z;

    return result;
}

vec3 v3Multiply(const vec3* v0, const vec3* v1) {
    vec3 result;
    result.x = v0->x * v1->x;
    result.y = v0->y * v1->y;
    result.z = v0->z * v1->z;
    return result;
}

vec3 v3MultiplyN(const vec3* v, double N) {
    vec3 result;

    result.x = v->x * N;
    result.y = v->y * N;
    result.z = v->z * N;

    return result;
}

vec3 v3Divide(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->x / v1->x;
    result.y = v0->y / v1->y;
    result.z = v0->z / v1->z;

    return result;
}

vec3 v3DivideN(const vec3* v, double N) {
    vec3 result;

    result.x = v->x / N;
    result.y = v->y / N;
    result.z = v->z / N;

    return result;
}

void v3Normalize(vec3* v) {
    *v = v3MultiplyN(v, v3InvLen(v));
}

double v3InvLen(const vec3* v) {
    return invSqrt(v3LenSquared(v));
}

double v3LenSquared(const vec3* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

double v3Dot(const vec3* v0, const vec3* v1) {
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;

}

bool nearZero(const vec3* v) {
    // Return true if the vector is close to zero in all dimensions.
    return (bool)((fabs(v->x) < 1e-8) && (fabs(v->y) < 1e-8) && (fabs(v->z) < 1e-8));
}

vec3 v3Cross(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->y * v1->z - v0->z * v1->y;
    result.y = v0->z * v1->x - v0->x * v1->z;
    result.z = v0->x * v1->y - v0->y * v1->x;

    return result;
}

vec3 v3Unit(const vec3* v) {
    return v3MultiplyN(v, v3InvLen(v));
}

vec3 v3Random() {
    vec3 result;

    result.x = randd();
    result.y = randd();
    result.z = randd();

    return result;
}

vec3 v3RandomRange(double min, double max) {
    vec3 result;

    result.x = randdRange(min, max);
    result.y = randdRange(min, max);
    result.z = randdRange(min, max);

    return result;
}

vec3 v3RandomUnitVec() {
    while (true) {
        vec3 p = v3RandomRange(-1, 1);
        double lensq = v3LenSquared(&p);
        
        if (1e-160 < lensq && lensq <= 1) {
            return v3Unit(&p);
        }      
    }
}

vec3 v3RandomInUnitDisk() {
    while (true) {
        vec3 p;
        p.x = randdRange(-1,1);
        p.y = randdRange(-1,1);
        p.z = 0;
        
        if (v3LenSquared(&p) < 1)
            return p;
    }
}

vec3 v3RandomOnHemisphere(const vec3* normal) {
    vec3 onv3UnitSphere = v3RandomUnitVec();
    
    if (v3Dot(&onv3UnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return onv3UnitSphere;
    
    return v3Negate(&onv3UnitSphere);
}

vec3 v3Reflect(const vec3* v, const vec3* n) {
    vec3 nMultv3Dot = v3MultiplyN(n, 2 * v3Dot(v, n));
    return v3Subtract(v, &nMultv3Dot);
}

vec3 v3Refract(vec3* uv, const vec3* n, double etaiOverEtat) {
    vec3 rayOutParallel;
    vec3 v3Negated = v3Negate(uv);
    double cos_theta = fmin(v3Dot(&v3Negated, n), 1.0);
    vec3 nMultCosTheta = v3MultiplyN(n, cos_theta);
    vec3 rayOutPerp = v3Add(&v3Negated, &nMultCosTheta);
    rayOutPerp = v3MultiplyN(&rayOutPerp, etaiOverEtat);
        
    rayOutParallel = v3DivideN(n, -invSqrt(fabs(1.0 - v3LenSquared(&rayOutPerp))));
    
    return v3Add(&rayOutPerp, &rayOutParallel);
}