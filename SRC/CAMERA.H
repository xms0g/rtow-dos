#ifndef camera_H
#define camera_H

#include <stddef.h>
#include "vec3.h"
#include "ray.h"

typedef struct camera {
    vec3 center;           // camera center
    vec3 pixel_00_loc;       // Location of pixel 0, 0
    vec3 pixel_delta_u;      // Offset to pixel to the right
    vec3 pixel_delta_v;      // Offset to pixel below
    vec3 defocus_disk_u;     // Defocus disk horizontal radius
    vec3 defocus_disk_v;     // Defocus disk vertical radius
    double defocus_angle;   // Variation angle of rays through each pixel

    ray (*get_ray)(int, int);
} camera;

camera* new_camera(double aspect_ratio, double vfov, double defocus_angle, double focus_dist, int width, const vec3* lookfrom, const vec3* lookat, const vec3* vup);

#endif