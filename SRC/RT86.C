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

color rayColor(const Ray* ray, int depth, const Scene* sc) {
    double a;
    HitRecord rec;
    vec3 unitDir;
    
    if (depth <= 0) {
        return ZERO_COLOR;
    }
    
    if (sc->hit(ray, 0.001, DBL_MAX, &rec)) {
        Ray scattered;
        color attenuation;
        if (rec.mat->vtable->scatter(rec.mat, ray, &rec, &attenuation, &scattered)) {
            vec3 scatteredCol = rayColor(&scattered, depth - 1, sc);
            return v3Multiply(&attenuation, &scatteredCol);
        }
       
        return ZERO_COLOR;
    }

    unitDir = v3Unit(&ray->direction);
    a = 0.5 * (unitDir.y + 1.0);

    return v3Lerp(&SKY_COLOR_BASE, &SKY_COLOR_UP, a);
}

void main(void) {
    int a, b, x, y, i;
    const vec3 groundCenter = newVec3(0.0, -1000, 0.0);
    const color groundColor = newVec3(0.5, 0.5, 0.5);
    const vec3 metalCenter = newVec3(4, 1, 0);
    const color metalColor = newVec3(0.7, 0.6, 0.5);
    const vec3 glassCenter = newVec3(0, 1, 0);
    const vec3 lambertCenter = newVec3(-4, 1, 0);
    const color lambertColor = newVec3(0.4, 0.2, 0.1);
    
    const Camera* cam = newCamera(ASPECT_RATIO, VFOV, DEFOCUS_ANGLE, FOCUS_DIST, WIDTH, &LOOKFROM, &LOOKAT, &VUP);
    const Scene* sc = newScene(100);

    const Sphere* const groundSphere = newSphere(groundCenter, 1000.0, (const Material*)newLambertian(groundColor));
    const Sphere* const metalSphere = newSphere(metalCenter, 1.0, (const Material*)newMetal(metalColor, 0.0));
    const Sphere* const glassSphere = newSphere(glassCenter, 1.0, (const Material*)newDielectric(1.5));
    const Sphere* const lambertSphere = newSphere(lambertCenter, 1.0, (const Material*)newLambertian(lambertColor));
   
    sc->add(groundSphere);
    sc->add(metalSphere);
    sc->add(glassSphere);
    sc->add(lambertSphere);
    
    for (a = -10; a < 10; a++) {
        for (b = -10; b < 10; b++) {
            vec3 temp = newVec3(4, 0.2, 0);
            vec3 center = newVec3(a + 0.9 * randd(), 0.2, b + 0.9 * randd());
            vec3 temp1 = v3Subtract(&center, &temp);
            double choose_mat = randd();
            
            if (v3Len(&temp1) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedoVec = v3Random();
                    vec3 albedoVec1 = v3Random();
                    vec3 albedo = v3Multiply(&albedoVec, &albedoVec1);

                    const Sphere* const sphere = newSphere(center, 0.2, (const Material*)newLambertian(albedo));
                    sc->add(sphere);
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = v3RandomRange(0.5, 1);
                    double fuzz = randdRange(0, 0.5);
                    
                    const Sphere* const sphere = newSphere(center, 0.2, (const Material*)newMetal(albedo, fuzz));
                    sc->add(sphere);
                } else {
                    // glass
                    const Sphere* const sphere = newSphere(center, 0.2, (const Material*)newDielectric(1.5));
                    sc->add(sphere);
                }
            }
        }
    }

    _initMode(MODE_VGA_13H);

    _waitvretrace();

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            vec3 scaledPixelColor;
            color pixelColor = newVec3(0, 0, 0);
            
            for (i = 0; i < SAMPLES_PER_PIXEL; ++i) {
                struct Ray r = cam->getRay(x, y);
                vec3 rayCol = rayColor(&r, MAX_DEPTH, sc);
                pixelColor = v3Add(&pixelColor, &rayCol);
            }
            
            scaledPixelColor = v3MultiplyN(&pixelColor, PIXEL_SAMPLES_SCALE);
            _putpixel(x, y, pixel2vga(&scaledPixelColor));
        }
    }

    getch();
    sc->clear();
    _initMode(MODE_VGA_3H);
}
