#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

struct Ray;
struct HitRecord;

typedef struct Material {
    bool (*scatter)(const struct Material*, const struct Ray*, const struct HitRecord*, color*, struct Ray*);
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

Lambertian* newLambertianMat(color albedo);

Metal* newMetalMat(color albedo, double fuzz);

Dielectric* newDielectricMat(double refractionIndex);

#endif