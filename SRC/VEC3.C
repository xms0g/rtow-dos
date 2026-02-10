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

vec3 v3MultiplyN(const vec3* v, double n) {
    vec3 result;

    result.x = v->x * n;
    result.y = v->y * n;
    result.z = v->z * n;

    return result;
}

vec3 v3Divide(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->x / v1->x;
    result.y = v0->y / v1->y;
    result.z = v0->z / v1->z;

    return result;
}

vec3 v3DivideN(const vec3* v, double n) {
    vec3 result;

    result.x = v->x / n;
    result.y = v->y / n;
    result.z = v->z / n;

    return result;
}

void normalize(vec3* v) {
    *v = v3MultiplyN(v, invLen(v));
}

double invLen(const vec3* v) {
    return invSqrt(lenSquared(v));
}

double lenSquared(const vec3* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

void sadd(vec3* v, double scalar) {
    v->x += scalar;
    v->y += scalar;
    v->z += scalar;
}

double dot(const vec3* v0, const vec3* v1) {
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;

}

int nearZero(const vec3* v) {
    // Return true if the vector is close to zero in all dimensions.
    return (int)((fabs(v->x) < 1e-8) && (fabs(v->y) < 1e-8) && (fabs(v->z) < 1e-8));
}

vec3 cross(const vec3* v0, const vec3* v1) {
    vec3 result;

    result.x = v0->y * v1->z - v0->z * v1->y;
    result.y = v0->z * v1->x - v0->x * v1->z;
    result.z = v0->x * v1->y - v0->y * v1->x;

    return result;
}

vec3 unit(const vec3* v) {
    return v3MultiplyN(v, invLen(v));
}

vec3 v3Random() {
    vec3 result;

    result.x = randd();
    result.y = randd();
    result.z = randd();

    return result;
}

vec3 v3RandomMinMax(double min, double max) {
    vec3 result;

    result.x = randdMinMax(min, max);
    result.y = randdMinMax(min, max);
    result.z = randdMinMax(min, max);

    return result;
}

vec3 v3RandomUnitVec() {
    while (true) {
        vec3 p = v3RandomMinMax(-1, 1);
        double lensq = lenSquared(&p);
        
        if (1e-160 < lensq && lensq <= 1) {
            return unit(&p);
        }      
    }
}

vec3 randomInUnitDisk() {
    while (true) {
        vec3 p;
        p.x = randdMinMax(-1,1);
        p.y = randdMinMax(-1,1);
        p.z = 0;
        
        if (lenSquared(&p) < 1)
            return p;
    }
}

vec3 randomOnHemisphere(const vec3* normal) {
    vec3 onUnitSphere = v3RandomUnitVec();
    
    if (dot(&onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return onUnitSphere;
    
    return v3Negate(&onUnitSphere);
}

vec3 reflect(const vec3* v, const vec3* n) {
    vec3 nMultDot = v3MultiplyN(n, 2 * dot(v, n));
    return v3Subtract(v, &nMultDot);
}

vec3 refract(vec3* uv, const vec3* n, double etaiOverEtat) {
    vec3 rayOutParallel;
    vec3 v3Negated = v3Negate(uv);
    double cos_theta = fmin(dot(&v3Negated, n), 1.0);
    vec3 nMultCosTheta = v3MultiplyN(n, cos_theta);
    vec3 rayOutPerp = v3Add(&v3Negated, &nMultCosTheta);
    rayOutPerp = v3MultiplyN(&rayOutPerp, etaiOverEtat);
        
    rayOutParallel = v3DivideN(n, -invSqrt(fabs(1.0 - lenSquared(&rayOutPerp))));
    
    return v3Add(&rayOutPerp, &rayOutParallel);
}