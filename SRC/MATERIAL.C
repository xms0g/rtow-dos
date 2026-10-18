#include "material.h"
#include <stddef.h>
#include "hitrcd.h"
#include "ray.h"
#include "math.h"

static bool lambertian_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered);
static bool metal_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered);
static bool dielectric_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered);
static double reflectance(double cosine, double refraction_index);

static const vtable lambertian_vtable = {lambertian_scatter};
static const vtable metal_vtable = {metal_scatter};
static const vtable dielectric_vtable = {dielectric_scatter};

const lambertian* new_lambertian(color albedo) {
    lambertian* mat = (lambertian*)malloc(sizeof(lambertian));

    if (mat == NULL) {
        return NULL;
    }
    mat->base.vtable = &lambertian_vtable;
    mat->albedo = albedo;
    
    return mat;
}

const metal* new_metal(color albedo, double fuzz) {
    metal* mat = (metal*)malloc(sizeof(metal));

    if (mat == NULL) {
        return NULL;
    }
    mat->base.vtable = &metal_vtable;
    mat->albedo = albedo;
    mat->fuzz = fuzz;
    
    return mat;
}

const dielectric* new_dielectric(double refraction_index) {
    dielectric* mat = (dielectric*)malloc(sizeof(dielectric));
    
    if (mat == NULL) {
        return NULL;
    }
    mat->base.vtable = &dielectric_vtable;
    mat->refraction_index = refraction_index;
    
    return mat;
}

bool lambertian_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered) {
    vec3 random_unit_vec = v3_random_unit_vec();
    vec3 scatter_dir = v3_add(&rec->normal, &random_unit_vec);
    (void)in;
    // Catch degenerate scatter direction
    if (v3_near_zero(&scatter_dir)) {
        scatter_dir = rec->normal;
    }
        
    scattered->origin = rec->p;
    scattered->direction = scatter_dir;
    *attenuation = ((lambertian*)mat)->albedo;
    
    return true;
}

bool metal_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered) {
    vec3 reflected = v3_reflect(&in->direction, &rec->normal);
    vec3 random_unit_vec = v3_random_unit_vec();
    vec3 reflected_unit = v3_unit(&reflected);
    vec3 fuzz_vec = v3_mul_n(&random_unit_vec, ((metal*)mat)->fuzz);
    reflected = v3_add(&reflected_unit, &fuzz_vec);
        
    scattered->origin = rec->p;
    scattered->direction = reflected;
        
    *attenuation = ((metal*)mat)->albedo;
    return (bool)(v3_dot(&scattered->direction, &rec->normal) > 0);
}

bool dielectric_scatter(const material* mat, const struct ray* in, const struct hit_record* rec, color* attenuation, struct ray* scattered) {
    vec3 dir;
    double ri = rec->front_face ? (1.0 / ((dielectric*)mat)->refraction_index) : ((dielectric*)mat)->refraction_index;
    vec3 unit_dir = v3_unit(&in->direction);
    vec3 negated = v3_negate(&unit_dir);
    double cos_theta = fmin(v3_dot(&negated, &rec->normal), 1.0);
    double sin_theta = 1.0 / inv_sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = (bool)(ri * sin_theta > 1.0);
    static vec3 attenuation = {1.0, 1.0, 1.0};

    if (cannot_refract || reflectance(cos_theta, ri) > randd()) {
        dir = v3_reflect(&unit_dir, &rec->normal);
    } else {
        dir = v3_refract(&unit_dir, &rec->normal, ri);
    }

    scattered->origin = rec->p;
    scattered->direction = dir;
    // the glass surface absorbs nothing.
    *attenuation = attenuation;

    return true;
}

double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}