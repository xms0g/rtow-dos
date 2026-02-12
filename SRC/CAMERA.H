#ifndef CAMERA_H
#define CAMERA_H

#include <stddef.h>
#include "vec3.h"
#include "ray.h"

typedef struct Camera {
    vec3 center;           // Camera center
    vec3 pixel00Loc;       // Location of pixel 0, 0
    vec3 pixelDeltaU;      // Offset to pixel to the right
    vec3 pixelDeltaV;      // Offset to pixel below
    double defocusAngle;   // Variation angle of rays through each pixel
    vec3 defocusDiskU;     // Defocus disk horizontal radius
    vec3 defocusDiskV;     // Defocus disk vertical radius

    Ray (*getRay)(const struct Camera*, int, int);
} Camera;

Camera* newCamera(double aspectRatio, double vfov, double defocusAngle, double focusDist, int imageWidth, const vec3* lookfrom, const vec3* lookat, const vec3* vup);

#endif