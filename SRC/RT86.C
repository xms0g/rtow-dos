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

// Camera Settings
#define ASPECT_RATIO (4.0 / 3.0)
#define VFOV 20.0
#define DEFOCUS_ANGLE 0.6
#define FOCUS_DIST 10.0

static const vec3 VUP = {0, 1, 0};
static const vec3 LOOKFROM = {13, 2, 3};
static const vec3 LOOKAT = {0, 0, 0};

static vec3 col1 = {1.0, 1.0, 1.0};
static vec3 col2 = {0.5, 0.7, 1.0};
static vec3 zeroColor = {0, 0, 0};

color rayColor(const Ray* ray, int depth) {
    double a;
    HitRecord rec;
    vec3 v3UnitDir, col1Scaled, col2Scaled;

    if (depth <= 0) {
        return zeroColor;
    }
    
    if (scHit(ray, 0.001, DBL_MAX, &rec)) {
        Ray scattered;
        color attenuation;
        switch (rec.mat->type) {
            case LAMBERTIAN:
                if (lambertianScatter((const Lambertian*)rec.mat, ray, &rec, &attenuation, &scattered)) {
                    vec3 scatteredCol = rayColor(&scattered, depth - 1);
                    return v3Multiply(&attenuation, &scatteredCol);
                }
                break;
            case METAL:
                if (metalScatter((const Metal*)rec.mat, ray, &rec, &attenuation, &scattered)) {
                    vec3 scatteredCol = rayColor(&scattered, depth - 1);
                    return v3Multiply(&attenuation, &scatteredCol);
                }
                break;
            case DIELECTRIC:
                if (dielectricScatter((const Dielectric*)rec.mat, ray, &rec, &attenuation, &scattered)) {
                    vec3 scatteredCol = rayColor(&scattered, depth - 1);
                    return v3Multiply(&attenuation, &scatteredCol);
                }
                break;
        }
       
        return zeroColor;
    }

    v3UnitDir = v3Unit(&ray->direction);
    a = 0.5 * (v3UnitDir.y + 1.0);
    col1Scaled = v3MultiplyN(&col1, 1.0 - a);
    col2Scaled = v3MultiplyN(&col2, a);

    return v3Add(&col1Scaled, &col2Scaled);
}

void main(void) {
    int a, b, x, y, i;
    Camera cam;
    vec3 groundCenter = {0.0, -1000, 0.0};
    color groundColor = {0.5, 0.5, 0.5};
    vec3 metalCenter = {4, 1, 0};
    color metalColor = {0.7, 0.6, 0.5};
    
    Sphere* groundSphere = malloc(sizeof(Sphere));
    Lambertian* groundMat = malloc(sizeof(Lambertian));
    Sphere* metalSphere = malloc(sizeof(Sphere));
    Metal* metalMat = malloc(sizeof(Metal));
    
    groundMat->base.type = LAMBERTIAN;
    groundMat->albedo = groundColor;
    
    groundSphere->center = groundCenter;
    groundSphere->radius = 1000.0;
    groundSphere->mat = (Material*)groundMat;

    metalMat->base.type = METAL;
    metalMat->albedo = metalColor;
    metalMat->fuzz = 0.0;
    
    metalSphere->center = metalCenter;
    metalSphere->radius = 1.0;
    metalSphere->mat = (Material*)metalMat;

    scInit(10);
    scAdd(groundSphere);
    scAdd(metalSphere);
    
    for (a = -5; a < 5; a++) {
        for (b = -5; b < 5; b++) {
            vec3 center, temp = {4, 0.2, 0}, temp1;
            double choose_mat = randd();
            
            center.x = a + 0.9 * randd();
            center.y = 0.2;
            center.z = b + 0.9 * randd();
        
            temp1 = v3Subtract(&center, &temp);
            if (pow(v3InvLen(&temp1), -1) > 0.9) {
                if (choose_mat < 0.8) {
                    Sphere* sphere = malloc(sizeof(Sphere));
                    Lambertian* lambertMat = malloc(sizeof(Lambertian));
                    // diffuse
                    vec3 albedoVec = v3Random();
                    vec3 albedoVec1 = v3Random();
                    vec3 albedo = v3Multiply(&albedoVec, &albedoVec1);
                    lambertMat->base.type = LAMBERTIAN;
                    lambertMat->albedo = albedo;
                    sphere->center = center;
                    sphere->radius = 0.2;
                    sphere->mat = (Material*)lambertMat;
                    scAdd(sphere);
                } else if (choose_mat < 0.95) {
                    // metal
                    Sphere* sphere = malloc(sizeof(Sphere));
                    Metal* metalMat = malloc(sizeof(Metal));
                    vec3 albedo = v3RandomRange(0.5, 1);
                    double fuzz = randdRange(0, 0.5);
                    metalMat->base.type = METAL;
                    metalMat->albedo = albedo;
                    metalMat->fuzz = fuzz;
                    sphere->center = center;
                    sphere->radius = 0.2;
                    sphere->mat = (Material*)metalMat;
                    scAdd(sphere);
                } else {
                    // glass
                    Sphere* sphere = malloc(sizeof(Sphere));
                    Dielectric* dielectricMat = malloc(sizeof(Dielectric));
                    dielectricMat->base.type = DIELECTRIC;
                    dielectricMat->refractionIndex = 1.5;
                   
                    sphere->center = center;
                    sphere->radius = 0.2;
                    sphere->mat = (Material*)dielectricMat;
                    scAdd(sphere);
                }
            }
        }
    }

    camInit(&cam, ASPECT_RATIO, VFOV, DEFOCUS_ANGLE, FOCUS_DIST, WIDTH, &LOOKFROM, &LOOKAT, &VUP);
   
    _initMode(MODE_VGA_13H);

    _waitvretrace();

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            vec3 scaledPixelColor;
            color pixelColor = {0, 0, 0};
            
            for (i = 0; i < SAMPLES_PER_PIXEL; ++i) {
                struct Ray r = getRay(&cam, x, y);
                vec3 rayCol = rayColor(&r, MAX_DEPTH);
                pixelColor = v3Add(&pixelColor, &rayCol);
            }
            
            scaledPixelColor = v3MultiplyN(&pixelColor, PIXEL_SAMPLES_SCALE);
            _putpixel(x, y, pixel2vga(&scaledPixelColor));
        }
    }

    getch();
    scClear();
    _initMode(MODE_VGA_3H);
}
