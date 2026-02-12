#include "scene.h"
#include "hitrcd.h"
#include "sphere.h"
#include "ray.h"

static void scAdd(Scene* this, const struct Sphere* object);
static void scClear(Scene* this);
static bool scHit(Scene* this, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec);

void scInit(Scene* this, int size) {
    saInit(&this->objects, size);
    this->add = scAdd;
    this->clear = scClear;
    this->hit = scHit;
}

void scAdd(Scene* this, const struct Sphere* object) {
    saPushback(&this->objects, object);
}

void scClear(Scene* this) {
    saFree(&this->objects);
}

 bool scHit(Scene* this, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec) {
    int i;
    bool hitAnything = false;
    double closestSoFar = tmax;

    for (i = 0; i < this->objects.count; ++i) {
        if (spHit(saAt(&this->objects, i), ray, tmin, closestSoFar, rec)) {
            hitAnything = true;
            closestSoFar = rec->t;
        }
    }

    return hitAnything;
 }