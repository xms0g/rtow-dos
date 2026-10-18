#ifndef material_H
#define material_H

#include "color.h"

struct ray;
struct hit_record;

typedef struct vtable {
    bool (*scatter)(const struct material*, const struct ray*, const struct hit_record*, color*, struct ray*);
} vtable;

typedef struct material {
    const vtable* vtable;
} material;

typedef struct lambertian {
    material base;
    color albedo;
} lambertian;

typedef struct metal {
    material base;
    color albedo;
    double fuzz;
} metal;

typedef struct dielectric {
    material base;
    double refraction_index;
} dielectric;

const lambertian* new_lambertian(color albedo);

const metal* new_metal(color albedo, double fuzz);

const dielectric* new_dielectric(double refraction_index);

#endif