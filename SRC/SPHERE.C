#include "sphere.h"
#include "material.h"
#include "math.h"
#include "hitrcd.h"
#include "ray.h"

Sphere* spInit(vec3 center, double radius, struct Material* mat) {
    Sphere* sphere = malloc(sizeof(Sphere));
    sphere->center = center;
    sphere->radius = radius;
    sphere->mat = mat;
    
    return sphere;
}

bool spHit(const Sphere* sphere, const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec) {
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

void saInit(SphereArray* sa, int size) {
    sa->count = 0;
    sa->capacity = size;
    sa->data = (Sphere*)malloc(size * sizeof(Sphere));
}

void saPushback(SphereArray* sa, const Sphere* sphere) {
    if (sa->count == sa->capacity) {
        sa->capacity *= 2;
        sa->data = (Sphere*)realloc(sa->data, sa->capacity * sizeof(Sphere));
    }

    sa->data[sa->count++] = *sphere;
}

void saRemove(SphereArray* sa, int index) {
    int i;

    if (index < 0 || index >= sa->count) {
        return;
    }
    
    for (i = index; i < sa->count - 1; i++) {
        sa->data[i] = sa->data[i + 1];
    }
    
    sa->count--;
}

Sphere* saAt(SphereArray* sa, int index) {
    if (index < 0 || index >= sa->count) {
        return NULL;
    }

    return &sa->data[index];
}

void saClear(SphereArray* sa) {
    memset(sa->data, 0, sa->count * sizeof(Sphere));
    sa->count = 0;
}

void saFree(SphereArray* sa) {
    int i;
    for (i = 0; i < sa->count; ++i) {
        free(sa->data[i].mat);
    }
    free(sa->data);
    sa->data = NULL;
}