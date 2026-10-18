#ifndef scene_H
#define scene_H

#include "bool.h"
#include "sphere.h"

struct ray;
struct hit_record;

typedef struct scene {
    int objectCount;
    sphere_array* const objects;

    void (*add)(const struct sphere*);
    void (*clear)(void);
    bool (*hit)(const struct ray*, double, double, struct hit_record*);
} scene;

scene* new_scene(int size);

#endif