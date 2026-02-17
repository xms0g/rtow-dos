#ifndef HITRECORD_H
#define HITRECORD_H

#include "bool.h"
#include "vec3.h"

struct Material;
struct Ray;

typedef struct HitRecord {
    vec3 p;
    vec3 normal;
    double t;
    bool frontFace;
    const struct Material* mat;
} HitRecord;

void setFaceNormal(HitRecord* rec, const struct Ray* ray, const vec3* outwardNormal);

#endif