#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "bool.h"

struct Material;
struct HitRecord;
struct Ray;

typedef struct Sphere {
    vec3 center;
    double radius;
    struct Material* mat;

    bool (*hit)(const struct Sphere*, const struct Ray*, double, double, struct HitRecord*);
} Sphere;

typedef struct SphereArray {    
    int count;      
    int capacity;   
    Sphere* data;
    
    void (*pushback)(struct SphereArray*, const struct Sphere*);
    void (*remove)(struct SphereArray*, int);
    struct Sphere* (*at)(struct SphereArray*, int);
    void (*clear)(struct SphereArray*);
    void (*free)(struct SphereArray*);
} SphereArray;

Sphere* newSphere(vec3 center, double radius, struct Material* mat);

SphereArray* newSphereArray(int size);


#endif