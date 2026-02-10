#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() {}

    Ray(const vec3* origin, const vec3* direction) : mOrigin(*origin), mDirection(*direction) {}

    const vec3* origin() const  { return &mOrigin; }
    
    const vec3* direction() const { return &mDirection; }

    vec3 at(double t) const {
        vec3 scaledDir = v3MultiplyN(&mDirection, t);
        return v3Add(&mOrigin, &scaledDir);
    }

private:
    vec3 mOrigin;
    vec3 mDirection;
};

#endif