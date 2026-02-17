#include "sphere.h"
#include <stddef.h>
#include "material.h"
#include "math.h"
#include "hitrcd.h"
#include "ray.h"

static bool spHit(const struct Sphere* sphere, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec);
static void saPushback(struct SphereArray* sa, const Sphere* sphere);
static void saRemove(struct SphereArray* sa, int index);
static Sphere* saAt(struct SphereArray* sa, int index);
static void saClear(struct SphereArray* sa);
static void saFree(struct SphereArray* sa);

Sphere* newSphere(vec3 center, double radius, struct Material* mat) {
    Sphere* sphere = (Sphere*)malloc(sizeof(Sphere));

    if (sphere == NULL) {
        return NULL;
    }

    sphere->center = center;
    sphere->radius = radius;
    sphere->mat = mat;
    sphere->hit = spHit;
    
    return sphere;
}

bool spHit(const struct Sphere* sphere, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec) {
    double sqrtd, root;
    vec3 outwardNormal;
    vec3 oc = v3Subtract(&sphere->center, &ray->origin);
    double a = v3Dot(&ray->direction, &ray->direction);
    double h = v3Dot(&ray->direction, &oc);
    double c = v3Dot(&oc, &oc) - sphere->radius * sphere->radius;
    double discriminant = h * h - a * c;
    
    if (discriminant < 0)
        return false;

    sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    root = (h - sqrtd) / a;
    if (root <= tmin || tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= tmin || tmax <= root)
            return false;
    }

    rec->t = root;
    rec->p = rayAt(ray, rec->t);
    
    outwardNormal = v3Subtract(&rec->p, &sphere->center);
    outwardNormal = v3DivideN(&outwardNormal, sphere->radius);
    setFaceNormal(rec, ray, &outwardNormal);
    rec->mat = sphere->mat;

    return true;
}

SphereArray* newSphereArray(int size) {
    SphereArray* sa = (SphereArray*)malloc(sizeof(SphereArray));

    if (sa == NULL) {
        return NULL;
    }

    sa->count = 0;
    sa->capacity = size;
    sa->data = (Sphere*)malloc(size * sizeof(Sphere));

    if (sa->data == NULL) {
        free(sa);
        return NULL;
    }
    
    sa->pushback = saPushback;
    sa->remove = saRemove;
    sa->at = saAt;
    sa->clear = saClear;
    sa->free = saFree;

    return sa;
}

void saPushback(struct SphereArray* sa, const Sphere* sphere) {
    if (sa->count == sa->capacity) {
        sa->capacity *= 2;
        sa->data = (Sphere*)realloc(sa->data, sa->capacity * sizeof(Sphere));
    }

    sa->data[sa->count++] = *sphere;
}

void saRemove(struct SphereArray* sa, int index) {
    int i;

    if (index < 0 || index >= sa->count) {
        return;
    }
    
    for (i = index; i < sa->count - 1; i++) {
        sa->data[i] = sa->data[i + 1];
    }
    
    sa->count--;
}

Sphere* saAt(struct SphereArray* sa, int index) {
    if (index < 0 || index >= sa->count) {
        return NULL;
    }

    return &sa->data[index];
}

void saClear(struct SphereArray* sa) {
    memset(sa->data, 0, sa->count * sizeof(Sphere));
    sa->count = 0;
}

void saFree(struct SphereArray* sa) {
    int i;
    for (i = 0; i < sa->count; ++i) {
        free(sa->data[i].mat);
    }
    
    free(sa->data);
    sa->data = NULL;
}