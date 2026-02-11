#include "hitrcd.h"
#include "material.h"
#include "ray.h"

void setFaceNormal(HitRecord* rec, const struct Ray* ray, const vec3* outwardNormal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have v3Unit length.
    
    rec->frontFace = (bool)(v3Dot(&ray->direction, outwardNormal) < 0);
    rec->normal = rec->frontFace ? *outwardNormal : v3Negate(outwardNormal);
}