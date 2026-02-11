#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

struct Ray;
struct HitRecord;

enum MaterialType {
    LAMBERTIAN,
    METAL,
    DIELECTRIC
};

typedef struct Material {
    enum MaterialType type;
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

bool lambertianScatter(const Lambertian* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

bool metalScatter(const Metal* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

bool dielectricScatter(const Dielectric* mat, const struct Ray* in, const struct HitRecord* rec, color* attenuation, struct Ray* scattered);

#endif