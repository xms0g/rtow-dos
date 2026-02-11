#include "material.h"
#include "hitrcd.h"
#include "ray.h"
#include "math.h"

bool lambertianScatter(const Lambertian* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 randomUnitVec = v3RandomUnitVec();
    vec3 scatterDir = v3Add(&rec->normal, &randomUnitVec);
    (void)in;
    // Catch degenerate scatter direction
    if (nearZero(&scatterDir)) {
        scatterDir = rec->normal;
    }
        
    scattered->origin = rec->p;
    scattered->direction = scatterDir;
    *attenuation = mat->albedo;
    
    return true;
}

bool metalScatter(const Metal* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 v3Reflected = v3Reflect(&in->direction, &rec->normal);
    vec3 randomUnitVec = v3RandomUnitVec();
    vec3 v3Reflectedv3Unit = v3Unit(&v3Reflected);
    vec3 fuzzVec = v3MultiplyN(&randomUnitVec, mat->fuzz);
    v3Reflected = v3Add(&v3Reflectedv3Unit, &fuzzVec);
        
    scattered->origin = rec->p;
    scattered->direction = v3Reflected;
        
    *attenuation = mat->albedo;
    return (bool)(v3Dot(&scattered->direction, &rec->normal) > 0);
}

bool dielectricScatter(const Dielectric* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 dir;
    double ri = rec->frontFace ? (1.0 / mat->refractionIndex) : mat->refractionIndex;
    vec3 v3UnitDir = v3Unit(&in->direction);
    vec3 v3Negated = v3Negate(&v3UnitDir);
    double cosTheta = fmin(v3Dot(&v3Negated, &rec->normal), 1.0);
    double invSinTheta = invSqrt(1.0 - cosTheta * cosTheta);
    bool cannotv3Refract = (bool)(ri / invSinTheta > 1.0);

    if (cannotv3Refract || v3Reflectance(cosTheta, ri) > randd()) {
        dir = v3Reflect(&v3UnitDir, &rec->normal);
    } else {
        dir = v3Refract(&v3UnitDir, &rec->normal, ri);
    }

    scattered->origin = rec->p;
    scattered->direction = dir;
     // the glass surface absorbs nothing.
    attenuation->x = 1.0;
    attenuation->y = 1.0;
    attenuation->z = 1.0;

    return true;
}

double v3Reflectance(double cosine, double refractionIndex) {
    // Use Schlick's approximation for v3Reflectance.
    double r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}