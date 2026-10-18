#ifndef sphere_H
#define sphere_H

#include "vec3.h"
#include "bool.h"

struct material;
struct hit_record;
struct ray;

typedef struct sphere {
    vec3 center;
    double radius;
    const struct material* mat;

    bool (*hit)(const struct sphere*, const struct ray*, double, double, struct hit_record*);
} sphere;

typedef struct sphere_array {    
    int count;      
    int capacity;   
    sphere* const data;
    
    void (*pushback)(struct sphere_array*, const struct sphere*);
    void (*remove)(struct sphere_array*, int);
    struct sphere* (*at)(struct sphere_array*, int);
    void (*clear)(struct sphere_array*);
    void (*free)(struct sphere_array*);
} sphere_array;

sphere* new_sphere(vec3 center, double radius, const struct material* mat);

sphere_array* new_sphere_array(int size);

#endif