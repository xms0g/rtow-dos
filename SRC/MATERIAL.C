#include "material.h"
#include <stddef.h>
#include "hitrcd.h"
#include "ray.h"
#include "math.h"

static bool lambertianScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);
static bool metalScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);
static bool dielectricScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);
static double reflectance(double cosine, double refractionIndex);

Lambertian* newLambertianMat(color albedo) {
    Lambertian* mat = malloc(sizeof(Lambertian));
    if (mat == NULL) {
        return NULL;
    }
    mat->base.scatter = lambertianScatter;
    mat->albedo = albedo;
    
    return mat;
}

Metal* newMetalMat(color albedo, double fuzz) {
    Metal* mat = malloc(sizeof(Metal));
    if (mat == NULL) {
        return NULL;
    }
    mat->base.scatter = metalScatter;
    mat->albedo = albedo;
    mat->fuzz = fuzz;
    
    return mat;
}

Dielectric* newDielectricMat(double refractionIndex) {
    Dielectric* mat = malloc(sizeof(Dielectric));
    if (mat == NULL) {
        return NULL;
    }
    mat->base.scatter = dielectricScatter;
    mat->refractionIndex = refractionIndex;
    
    return mat;
}

bool lambertianScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 randomUnitVec = v3RandomUnitVec();
    vec3 scatterDir = v3Add(&rec->normal, &randomUnitVec);
    (void)in;
    // Catch degenerate scatter direction
    if (nearZero(&scatterDir)) {
        scatterDir = rec->normal;
    }
        
    scattered->origin = rec->p;
    scattered->direction = scatterDir;
    *attenuation = ((Lambertian*)mat)->albedo;
    
    return true;
}

bool metalScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 reflected = v3Reflect(&in->direction, &rec->normal);
    vec3 randomUnitVec = v3RandomUnitVec();
    vec3 reflectedUnit = v3Unit(&reflected);
    vec3 fuzzVec = v3MultiplyN(&randomUnitVec, ((Metal*)mat)->fuzz);
    reflected = v3Add(&reflectedUnit, &fuzzVec);
        
    scattered->origin = rec->p;
    scattered->direction = reflected;
        
    *attenuation = ((Metal*)mat)->albedo;
    return (bool)(v3Dot(&scattered->direction, &rec->normal) > 0);
}

bool dielectricScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered) {
    vec3 dir;
    double ri = rec->frontFace ? (1.0 / ((Dielectric*)mat)->refractionIndex) : ((Dielectric*)mat)->refractionIndex;
    vec3 unitDir = v3Unit(&in->direction);
    vec3 negated = v3Negate(&unitDir);
    double cosTheta = fmin(v3Dot(&negated, &rec->normal), 1.0);
    double invSinTheta = invSqrt(1.0 - cosTheta * cosTheta);
    bool cannotRefract = (bool)(ri / invSinTheta > 1.0);

    if (cannotRefract || reflectance(cosTheta, ri) > randd()) {
        dir = v3Reflect(&unitDir, &rec->normal);
    } else {
        dir = v3Refract(&unitDir, &rec->normal, ri);
    }

    scattered->origin = rec->p;
    scattered->direction = dir;
     // the glass surface absorbs nothing.
    attenuation->x = 1.0;
    attenuation->y = 1.0;
    attenuation->z = 1.0;

    return true;
}

double reflectance(double cosine, double refractionIndex) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}