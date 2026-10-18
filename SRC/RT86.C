#include <conio.h>
#include <float.h>
#include "color.h"
#include "math.h"
#include "vga.h"
#include "ray.h"
#include "vec3.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "hitrcd.h"

#define WIDTH 320
#define HEIGHT 200
#define SAMPLES_PER_PIXEL 10
#define MAX_DEPTH 10
#define PIXEL_SAMPLES_SCALE (1.0 / SAMPLES_PER_PIXEL)
#define VFOV 20.0
#define DEFOCUS_ANGLE 0.6
#define FOCUS_DIST 10.0
#define ASPECT_RATIO (4.0 / 3.0)

static const vec3 VUP = {0, 1, 0};
static const vec3 LOOKFROM = {13, 2, 3};
static const vec3 LOOKAT = {0, 0, 0};
static const color SKY_COLOR_BASE = {1.0, 1.0, 1.0};
static const color SKY_COLOR_UP = {0.5, 0.7, 1.0};
static const color ZERO_COLOR = {0, 0, 0};

color rayColor(const ray* ray, int depth, const scene* sc) {
    double a;
    hit_record rec;
    vec3 unit_dir;
    
    if (depth <= 0) {
        return ZERO_COLOR;
    }
    
    if (sc->hit(ray, 0.001, DBL_MAX, &rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->vtable->scatter(rec.mat, ray, &rec, &attenuation, &scattered)) {
            vec3 scatteredCol = rayColor(&scattered, depth - 1, sc);
            return v3_mul(&attenuation, &scatteredCol);
        }
       
        return ZERO_COLOR;
    }

    unit_dir = v3_unit(&ray->direction);
    a = 0.5 * (unit_dir.y + 1.0);

    return v3_lerp(&SKY_COLOR_BASE, &SKY_COLOR_UP, a);
}

void main(void) {
    int a, b, x, y, i;
    const vec3 ground_center = new_vec3(0.0, -1000, 0.0);
    const color ground_color = new_vec3(0.5, 0.5, 0.5);
    const vec3 metal_center = new_vec3(4, 1, 0);
    const color metal_color = new_vec3(0.7, 0.6, 0.5);
    const vec3 glass_center = new_vec3(0, 1, 0);
    const vec3 lambert_center = new_vec3(-4, 1, 0);
    const color lambert_color = new_vec3(0.4, 0.2, 0.1);
    
    const camera* cam = new_camera(ASPECT_RATIO, VFOV, DEFOCUS_ANGLE, FOCUS_DIST, WIDTH, &LOOKFROM, &LOOKAT, &VUP);
    const scene* sc = new_scene(100);

    const sphere* const ground_sphere = new_sphere(ground_center, 1000.0, (const material*)new_lambertian(ground_color));
    const sphere* const metal_sphere = new_sphere(metal_center, 1.0, (const material*)new_metal(metal_color, 0.0));
    const sphere* const glass_sphere = new_sphere(glass_center, 1.0, (const material*)new_dielectric(1.5));
    const sphere* const lambert_sphere = new_sphere(lambert_center, 1.0, (const material*)new_lambertian(lambert_color));
   
    sc->add(ground_sphere);
    sc->add(metal_sphere);
    sc->add(glass_sphere);
    sc->add(lambert_sphere);
    
    for (a = -10; a < 10; a++) {
        for (b = -10; b < 10; b++) {
            vec3 temp = new_vec3(4, 0.2, 0);
            vec3 center = new_vec3(a + 0.9 * randd(), 0.2, b + 0.9 * randd());
            vec3 temp1 = v3_sub(&center, &temp);
            double choose_mat = randd();
            
            if (v3_len(&temp1) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedo_vec = v3_random();
                    vec3 albedo_vec1 = v3_random();
                    vec3 albedo = v3_mul(&albedo_vec, &albedo_vec1);

                    const sphere* const sphere = new_sphere(center, 0.2, (const material*)new_lambertian(albedo));
                    sc->add(sphere);
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = v3_random_range(0.5, 1);
                    double fuzz = randd_range(0, 0.5);
                    
                    const sphere* const sphere = new_sphere(center, 0.2, (const material*)new_metal(albedo, fuzz));
                    sc->add(sphere);
                } else {
                    // glass
                    const sphere* const sphere = new_sphere(center, 0.2, (const material*)new_dielectric(1.5));
                    sc->add(sphere);
                }
            }
        }
    }

    _init_mode(MODE_VGA_13H);

    _wait_vretrace();

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            vec3 scaled_pix_color;
            color pix_color = new_vec3(0, 0, 0);
            
            for (i = 0; i < SAMPLES_PER_PIXEL; ++i) {
                struct ray r = cam->get_ray(x, y);
                vec3 ray_color = rayColor(&r, MAX_DEPTH, sc);
                pix_color = v3_add(&pix_color, &ray_color);
            }
            
            scaled_pix_color = v3_mul_n(&pix_color, PIXEL_SAMPLES_SCALE);
            _put_pixel(x, y, pixel2vga(&scaled_pix_color));
        }
    }

    getch();
    sc->clear();
    _init_mode(MODE_VGA_3H);
}
