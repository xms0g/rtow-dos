#ifndef hit_record_H
#define hit_record_H

#include "bool.h"
#include "vec3.h"

struct material;
struct ray;

typedef struct hit_record {
    vec3 p;
    vec3 normal;
    double t;
    bool front_face;
    const struct material* mat;
} hit_record;

void set_face_normal(hit_record* rec, const struct ray* ray, const vec3* outward_normal);

#endif