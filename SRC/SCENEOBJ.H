#ifndef SCENEOBJ_H
#define SCENEOBJ_H

#include "ray.h"
#include "bool.h"

class Material;
struct HitRecord {
    vec3 p;
    vec3 normal;
    Material* mat;
    double t;
    bool frontFace;

    void setFaceNormal(const struct Ray* ray, const vec3* outwardNormal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        frontFace = (bool)(dot(rayDirection(ray), outwardNormal) < 0);
        normal = frontFace ? *outwardNormal : v3Negate(outwardNormal);
    }
};

class SceneObject {
public:
    virtual ~SceneObject() {}

    virtual bool hit(const struct Ray* ray, double tmin, double tmax, HitRecord* rec) const = 0;
};

#endif