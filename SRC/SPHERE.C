#include "sphere.h"
#include <stddef.h>
#include "material.h"
#include "math.h"
#include "hitrcd.h"
#include "ray.h"

static bool sp_hit(const struct sphere* sphere, const struct ray* ray, double tmin, double tmax, struct hit_record* rec);
static void sa_pushback(struct sphere_array* sa, const sphere* sphere);
static void sa_remove(struct sphere_array* sa, int index);
static sphere* sa_at(struct sphere_array* sa, int index);
static void sa_clear(struct sphere_array* sa);
static void sa_free(struct sphere_array* sa);

sphere* new_sphere(vec3 center, double radius, const struct material* mat) {
    sphere* sphere = (sphere*)malloc(sizeof(sphere));

    if (sphere == NULL) {
        return NULL;
    }

    sphere->center = center;
    sphere->radius = radius;
    sphere->mat = mat;
    sphere->hit = sp_hit;
    
    return sphere;
}

bool sp_hit(const struct sphere* sphere, const struct ray* ray, double tmin, double tmax, struct hit_record* rec) {
    double sqrtd, root;
    vec3 outward_normal;
    vec3 oc = v3_sub(&sphere->center, &ray->origin);
    double a = v3_dot(&ray->direction, &ray->direction);
    double h = v3_dot(&ray->direction, &oc);
    double c = v3_dot(&oc, &oc) - sphere->radius * sphere->radius;
    double discriminant = h * h - a * c;
    
    if (discriminant < 0)
        return false;

    sqrtd = 1.0 / inv_sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    root = (h - sqrtd) / a;
    if (root <= tmin || tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= tmin || tmax <= root)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(ray, rec->t);
    
    outward_normal = v3_sub(&rec->p, &sphere->center);
    outward_normal = v3_divide_n(&outward_normal, sphere->radius);
    set_face_normal(rec, ray, &outward_normal);
    rec->mat = sphere->mat;

    return true;
}

sphere_array* new_sphere_array(int size) {
    sphere_array* sa = (sphere_array*)malloc(sizeof(sphere_array));

    if (sa == NULL) {
        return NULL;
    }

    sa->count = 0;
    sa->capacity = size;
    sa->data = (sphere*)malloc(size * sizeof(sphere));

    if (sa->data == NULL) {
        free(sa);
        return NULL;
    }
    
    sa->pushback = sa_pushback;
    sa->remove = sa_remove;
    sa->at = sa_at;
    sa->clear = sa_clear;
    sa->free = sa_free;

    return sa;
}

void sa_pushback(struct sphere_array* sa, const sphere* sphere) {
    if (sa->count == sa->capacity) {
        sa->capacity *= 2;
        sa->data = (sphere*)realloc(sa->data, sa->capacity * sizeof(sphere));
    }

    sa->data[sa->count++] = *sphere;
}

void sa_remove(struct sphere_array* sa, int index) {
    int i;

    if (index < 0 || index >= sa->count) {
        return;
    }
    
    for (i = index; i < sa->count - 1; i++) {
        sa->data[i] = sa->data[i + 1];
    }
    
    sa->count--;
}

sphere* sa_at(struct sphere_array* sa, int index) {
    if (index < 0 || index >= sa->count) {
        return NULL;
    }

    return &sa->data[index];
}

void sa_clear(struct sphere_array* sa) {
    memset(sa->data, 0, sa->count * sizeof(sphere));
    sa->count = 0;
}

void sa_free(struct sphere_array* sa) {
    int i;
    for (i = 0; i < sa->count; ++i) {
        free(sa->data[i].mat);
    }
    
    free(sa->data);
    sa->data = NULL;
}