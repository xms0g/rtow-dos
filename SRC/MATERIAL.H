#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

struct Ray;
struct HitRecord;

typedef struct Material {
    bool (*scatter)(const struct Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);
} Material;

typedef struct Lambertian {
    Material base;
    color albedo;
} Lambertian;

typedef struct Metal {
    Material base;
    color albedo;
    double fuzz;
} Metal;

typedef struct Dielectric {
    Material base;
    double refractionIndex;
} Dielectric;

Lambertian* matLambertianInit(color albedo);

Metal* matMetalInit(color albedo, double fuzz);

Dielectric* matDielectricInit(double refractionIndex);

#endif