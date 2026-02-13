#include "scene.h"
#include <stddef.h>
#include "hitrcd.h"
#include "sphere.h"
#include "material.h"
#include "ray.h"

static void scAdd(struct Scene* this, const struct Sphere* object);
static void scClear(struct Scene* this);
static bool scHit(struct Scene* this, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec);

Scene* newScene(int size) {
    Scene* this = (Scene*)malloc(sizeof(Scene));

    if (this == NULL) {
        return NULL;
    }
    this->add = scAdd;
    this->clear = scClear;
    this->hit = scHit;
    
    this->objects = newSphereArray(size);
    
    if (this->objects == NULL) {
        free(this);
        return NULL;
    }
    return this;
}

void scAdd(struct Scene* this, const struct Sphere* object) {
    this->objects->pushback(this->objects, object);
}

void scClear(struct Scene* this) {
    this->objects->free(this->objects);
    free(this->objects);
    this->objects = NULL;
}

 bool scHit(struct Scene* this, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec) {
    int i;
    bool hitAnything = false;
    double closestSoFar = tmax;

    for (i = 0; i < this->objects->count; ++i) {
        const Sphere* sphere = this->objects->at(this->objects, i);

        if (sphere->hit(sphere, ray, tmin, closestSoFar, rec)) {
            hitAnything = true;
            closestSoFar = rec->t;
        }
    }

    return hitAnything;
 }