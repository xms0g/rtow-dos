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
} Sphere;

typedef struct {    
    int count;      
    int capacity;   
    Sphere* data;     
} SphereArray;

bool spHit(const Sphere* sphere, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec);

void saInit(SphereArray* sa, int size);
void saPushback(SphereArray* sa, const Sphere* sphere);
void saRemove(SphereArray* sa, int index);
Sphere* saAt(SphereArray* sa, int index);
void saClear(SphereArray* sa);
void saFree(SphereArray* sa);

#endif