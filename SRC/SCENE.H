#ifndef SCENE_H
#define SCENE_H

#include "bool.h"
#include "sphere.h"

struct Ray;
struct HitRecord;

typedef struct Scene {
    int objectCount;
    SphereArray* objects;

    void (*add)(const struct Sphere*);
    void (*clear)(void);
    bool (*hit)(const struct Ray*, double, double, struct HitRecord*);
} Scene;

Scene* newScene(int size);

#endif