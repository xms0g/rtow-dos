#include "scene.h"
#include "hitrcd.h"
#include "sphere.h"
#include "ray.h"

static SphereArray objects;

void scInit(int size) {
    saInit(&objects, size);
}

void scAdd(const struct Sphere* object) {
    saPushback(&objects, object);
}

void scClear() {
    saFree(&objects);
}

 bool scHit(const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec) {
    int i;
    bool hitAnything = false;
    double closestSoFar = tmax;

    for (i = 0; i < objects.count; ++i) {
        if (spHit(saAt(&objects, i), ray, tmin, closestSoFar, rec)) {
            hitAnything = true;
            closestSoFar = rec->t;
        }
    }

    return hitAnything;
 }