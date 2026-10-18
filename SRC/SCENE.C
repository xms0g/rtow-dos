#include "scene.h"
#include <stddef.h>
#include "hitrcd.h"
#include "sphere.h"
#include "material.h"
#include "ray.h"

static scene this;

static void sc_add(const struct sphere* object);
static bool sc_hit(const struct ray* ray, double tmin, double tmax, struct hit_record* rec);
static void sc_clear(void);

scene* new_scene(int size) {
    this.add = sc_add;
    this.clear = sc_clear;
    this.hit = sc_hit;
    
    this.objects = new_sphere_array(size);
    
    if (this.objects == NULL) {
        return NULL;
    }
    
    return &this;
}

void sc_add(const struct sphere* object) {
    this.objects->pushback(this.objects, object);
}

void sc_clear(void) {
    this.objects->free(this.objects);
    free(this.objects);
    this.objects = NULL;
}

 bool sc_hit(const struct ray* ray, double tmin, double tmax, struct hit_record* rec) {
    int i;
    bool hit_anything = false;
    double closest = tmax;

    for (i = 0; i < this.objects->count; ++i) {
        const sphere* sphere = this.objects->at(this.objects, i);

        if (sphere->hit(sphere, ray, tmin, closest, rec)) {
            hit_anything = true;
            closest = rec->t;
        }
    }

    return hit_anything;
 }