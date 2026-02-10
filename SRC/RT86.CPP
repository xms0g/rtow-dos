#include <conio.h>
#include <float.h>
#include "color.h"
#include "VGA.H"
#include "ray.h"
#include "vec3.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#define WIDTH 320
#define HEIGHT 200
#define SAMPLES_PER_PIXEL 10
#define PIXEL_SAMPLES_SCALE 1.0 / SAMPLES_PER_PIXEL
#define MAX_DEPTH 10
// Camera Settings
#define ASPECT_RATIO    4.0 / 3.0
#define VFOV            20
#define DEFOCUS_ANGLE   0.6
#define FOCUS_DIST      10.0

static const vec3 VUP = {0, 1, 0};
static const vec3 LOOKFROM = {13, 2, 3};
static const vec3 LOOKAT = {0, 0, 0};

color rayColor(const Ray* ray, int depth, const Scene* scene) {
    if (depth <= 0) {
        vec3 zeroColor = {0, 0, 0};
        return zeroColor;
    }

    HitRecord rec;
    if (scene->hit(ray, 0.001, DBL_MAX, &rec)) {
        Ray scattered;
        color attenuation;
        if (rec.mat->scatter(ray, &rec, &attenuation, &scattered)) {
            vec3 scatteredCol = rayColor(&scattered, depth - 1, scene);
            return v3Multiply(&attenuation, &scatteredCol);
        }
       
        vec3 zeroColor = {0, 0, 0};
        return zeroColor;
    }

    vec3 unitDir = unit(ray->direction());
    double a = 0.5 * (unitDir.y + 1.0);
    vec3 col1 = {1.0, 1.0, 1.0};
    vec3 col2 = {0.5, 0.7, 1.0};
    vec3 col1Scaled = v3MultiplyN(&col1, 1.0 - a);
    vec3 col2Scaled = v3MultiplyN(&col2, a);

    return v3Add(&col1Scaled, &col2Scaled);
}

void main(void) {
    Scene scene;
    vec3 center = {0.0, -1000, 0.0};
    color metalColor = {0.5, 0.5, 0.5};
    scene.add(new Sphere(&center, 1000.0, new Metal(&metalColor, 0.5)));   // Ground
    
    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            vec3 center = {a + 0.9 * randd(), 0.2, b + 0.9 * randd()};
            vec3 temp = {0, 0, 0};
            
            double choose_mat = randd();

            vec3 temp1 = v3Subtract(&center, &temp);
            if (pow(invLen(&temp1), -1) > 0.9) {
                Material* sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedoVec = v3Random();
                    vec3 albedoVec1 = v3Random();
                    vec3 albedo = v3Multiply(&albedoVec, &albedoVec1);
                    sphere_material = new Lambertian(&albedo);
                    scene.add(new Sphere(&center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = v3RandomMinMax(0.5, 1);
                    double fuzz = randdMinMax(0, 0.5);
                    sphere_material = new Metal(&albedo, fuzz);
                    scene.add(new Sphere(&center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = new Dielectric(1.5);
                    scene.add(new Sphere(&center, 0.2, sphere_material));
                }
            }
        }
    }

    // scene.add(new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
    // scene.add(new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(color(0.4, 0.2, 0.1))));
    vec3 center1 = {4, 1, 0};
    color metalColor1 = {0.7, 0.6, 0.5};

    scene.add(new Sphere(&center1, 1.0, new Metal(&metalColor1, 0.0)));

    Camera cam(ASPECT_RATIO, VFOV, DEFOCUS_ANGLE, FOCUS_DIST, WIDTH, &LOOKFROM, &LOOKAT, &VUP);
   
    _initMode(MODE_VGA_13H);

    _waitvretrace();

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            color pixelColor = {0, 0, 0};
            
            for (int i = 0; i < SAMPLES_PER_PIXEL; ++i) {
                Ray r = cam.getRay(x, y);
                vec3 rayCol = rayColor(&r, MAX_DEPTH, &scene);
                pixelColor = v3Add(&pixelColor, &rayCol);
            }
            
            vec3 scaledPixelColor = v3MultiplyN(&pixelColor, PIXEL_SAMPLES_SCALE);
            _putpixel(x, y, pixel2vga(&scaledPixelColor));
        }
    }

    getch();
    _initMode(MODE_VGA_3H);
}
