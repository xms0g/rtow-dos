#include "sphere.h"
#include "material.h"

Sphere::~Sphere() {
    delete mMat;
}

bool Sphere::hit(const struct Ray* ray, double tmin, double tmax, HitRecord* rec) const {
    vec3 oc = v3Subtract(&mCenter, rayOrigin(ray));
    double a = dot(rayDirection(ray), rayDirection(ray));
    double h = dot(rayDirection(ray), &oc);
    double c = dot(&oc, &oc) - mRadius * mRadius;
    double discriminant = h * h - a * c;
    
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (root <= tmin || tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= tmin || tmax <= root)
            return false;
    }

    rec->t = root;
    rec->p = rayAt(ray, rec->t);
    
    vec3 outwardNormal = v3Subtract(&rec->p, &mCenter);
    outwardNormal = v3DivideN(&outwardNormal, mRadius);
    rec->setFaceNormal(ray, &outwardNormal);
    rec->mat = mMat;

    return true;
}