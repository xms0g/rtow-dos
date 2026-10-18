#include "hitrcd.h"
#include "material.h"
#include "ray.h"

void set_face_normal(hit_record* rec, const struct ray* ray, const vec3* outward_normal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have v3_unit length.
    
    rec->front_face = (bool)(v3_dot(&ray->direction, outward_normal) < 0);
    rec->normal = rec->front_face ? *outward_normal : v3_negate(outward_normal);
}