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

bool lambertianScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

bool metalScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

bool dielectricScatter(const Material* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

#endif