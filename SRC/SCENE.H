#ifndef SCENE_H
#define SCENE_H

#include "bool.h"
#include "sphere.h"

struct Ray;
struct HitRecord;

typedef struct Scene {
    int objectCount;
    SphereArray* objects;

    void (*add)(struct Scene*, const struct Sphere*);
    void (*clear)(struct Scene*);
    bool (*hit)(struct Scene*, const struct Ray*, double, double, struct HitRecord*);
} Scene;

Scene* newScene(int size);

#endif