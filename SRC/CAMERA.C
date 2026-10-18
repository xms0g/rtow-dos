#include "camera.h"
#include <stddef.h>
#include "math.h"

static camera this;

static vec3 sampleSquare();
static vec3 defocusDiskSample(const camera* cam);
static ray get_ray(int x, int y);

camera* new_camera(double aspect_ratio, double vfov, double defocus_angle, double focus_dist, int width, const vec3* lookfrom, const vec3* lookat, const vec3* vup) {
    int height;
    double theta, h, viewport_height, viewport_width, defocus_radius;
    vec3 sub_look_from_at, w, cross_vup_w, u, v, viewport_u, viewport_v, add_uv, half_add_uv, mul_w_focus_dist, viewport_upper_left, pixel_00_offset;

    height = (int)(width / aspect_ratio);
    height = (height < 1) ? 1 : height;
    theta = degree2radian(vfov);
    h = tan(theta/2);
    viewport_height = 2.0 * h * focus_dist;
    viewport_width = viewport_height * ((double)width/height);

    this.center = *lookfrom;
    this.defocus_angle = defocus_angle;

    // Calculate the u,v,w v3_unit basis vectors for the camera coordinate frame.
    sub_look_from_at = v3_sub(lookfrom, lookat);
    w = v3_unit(&sub_look_from_at);
    cross_vup_w = v3_cross(vup, &w);
    u = v3_unit(&cross_vup_w);
    v = v3_cross(&w, &u);

    // Calculate the vectors av3_cross the horizontal and down the vertical viewport edges.
    viewport_u = v3_mul_n(&u, viewport_width);
    viewport_v = v3_mul_n(&v, -viewport_height);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    this.pixel_delta_u = v3_divide_n(&viewport_u, width);
    this.pixel_delta_v = v3_divide_n(&viewport_v, height);

    // Calculate the location of the upper left pixel.
    add_uv = v3_add(&viewport_u, &viewport_v);
    half_add_uv = v3_mul_n(&add_uv, 0.5);
    mul_w_focus_dist = v3_mul_n(&w, focus_dist);
    viewport_upper_left = v3_sub(&this.center, &mul_w_focus_dist);
    viewport_upper_left = v3_sub(&viewport_upper_left, &half_add_uv);
    pixel_00_offset = v3_add(&this.pixel_delta_u, &this.pixel_delta_v);
    pixel_00_offset = v3_mul_n(&pixel_00_offset, 0.5);
    this.pixel_00_loc = v3_add(&viewport_upper_left, &pixel_00_offset);

    // Calculate the camera defocus disk basis vectors.
    defocus_radius = focus_dist * tan(degree2radian(defocus_angle / 2));
    this.defocus_disk_u = v3_mul_n(&u, defocus_radius);
    this.defocus_disk_v = v3_mul_n(&v, defocus_radius);

    this.get_ray = get_ray;
    
    return &this;
}

ray get_ray(int x, int y) {
    ray result;
    vec3 ray_orig, ray_dir;
    vec3 offset = sampleSquare();
    vec3 pix_delta_u_mult = v3_mul_n(&this.pixel_delta_u, x + offset.x);
    vec3 pix_delta_v_mult = v3_mul_n(&this.pixel_delta_v, y + offset.y);
    vec3 sum_pix_deltas = v3_add(&pix_delta_u_mult, &pix_delta_v_mult);
    vec3 pixel_sample = v3_add(&this.pixel_00_loc, &sum_pix_deltas);

    ray_orig = (this.defocus_angle <= 0) ? this.center : defocusDiskSample(&this);
    ray_dir = v3_sub(&pixel_sample, &ray_orig);
    
    result.origin = ray_orig;
    result.direction = ray_dir;
    
    return result;
}

vec3 sampleSquare() {
    vec3 result;
    
    result.x = randd() - 0.5;
    result.y = randd() - 0.5;
    result.z = 0;
    
    return result;
}

vec3 defocusDiskSample(const camera* cam) {
    // Returns a random point in the camera defocus disk.
    vec3 p = v3_random_in_unit_disk();
    vec3 defocus_u = v3_mul_n(&cam->defocus_disk_u, p.x);
    vec3 defocus_v = v3_mul_n(&cam->defocus_disk_v, p.y);
    vec3 disk_sample = v3_add(&defocus_u, &defocus_v);

    return v3_add(&cam->center, &disk_sample);
}