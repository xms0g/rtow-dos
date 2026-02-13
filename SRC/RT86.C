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

color rayColor(const Ray* ray, int depth, Scene* sc) {
    double a;
    HitRecord rec;
    vec3 unitDir, col1Scaled, col2Scaled;

    if (depth <= 0) {
        return zeroColor;
    }
    
    if (sc->hit(sc, ray, 0.001, DBL_MAX, &rec)) {
        Ray scattered;
        color attenuation;
        if (rec.mat->scatter(rec.mat, ray, &rec, &attenuation, &scattered)) {
            vec3 scatteredCol = rayColor(&scattered, depth - 1, sc);
            return v3Multiply(&attenuation, &scatteredCol);
        }
       
        return zeroColor;
    }

    unitDir = v3Unit(&ray->direction);
    a = 0.5 * (unitDir.y + 1.0);
    col1Scaled = v3MultiplyN(&col1, 1.0 - a);
    col2Scaled = v3MultiplyN(&col2, a);

    return v3Add(&col1Scaled, &col2Scaled);
}

void main(void) {
    int a, b, x, y, i;
    Camera* cam = newCamera(ASPECT_RATIO, VFOV, DEFOCUS_ANGLE, FOCUS_DIST, WIDTH, &LOOKFROM, &LOOKAT, &VUP);
    Scene* sc = newScene(30);
    vec3 groundCenter = {0.0, -1000, 0.0};
    color groundColor = {0.5, 0.5, 0.5};
    vec3 metalCenter = {4, 1, 0};
    color metalColor = {0.7, 0.6, 0.5};
    vec3 glassCenter = {0, 1, 0};
    Metal* metalMat;
    Dielectric* dielectricMat;
    Sphere* metalSphere, *glassSphere;

    Lambertian* groundMat = newLambertian(groundColor);
    Sphere* groundSphere = newSphere(groundCenter, 1000.0, (Material*)groundMat);

    sc->add(sc, groundSphere);
    
    for (a = -10; a < 10; a++) {
        for (b = -10; b < 10; b++) {
            vec3 center, temp = {4, 0.2, 0}, temp1;
            double choose_mat = randd();
            
            center.x = a + 0.9 * randd();
            center.y = 0.2;
            center.z = b + 0.9 * randd();
        
            temp1 = v3Subtract(&center, &temp);
            if (v3Len(&temp1) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedoVec = v3Random();
                    vec3 albedoVec1 = v3Random();
                    vec3 albedo = v3Multiply(&albedoVec, &albedoVec1);

                    Lambertian* lambertMat = newLambertian(albedo);
                    Sphere* sphere = newSphere(center, 0.2, (Material*)lambertMat);
                  
                    sc->add(sc, sphere);
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = v3RandomRange(0.5, 1);
                    double fuzz = randdRange(0, 0.5);
                    
                    Metal* metalMat = newMetal(albedo, fuzz);
                    Sphere* sphere = newSphere(center, 0.2, (Material*)metalMat);
                    
                    sc->add(sc, sphere);
                } else {
                    // glass
                    Dielectric* dielectricMat = newDielectric(1.5);
                    Sphere* sphere = newSphere(center, 0.2, (Material*)dielectricMat);
                    
                    sc->add(sc, sphere);
                }
            }
        }
    }

    metalMat = newMetal(metalColor, 0.0);
    metalSphere = newSphere(metalCenter, 1.0, (Material*)metalMat);

    dielectricMat = newDielectric(1.5);
    glassSphere = newSphere(glassCenter, 1.0, (Material*)dielectricMat);
   
    sc->add(sc, metalSphere);
    sc->add(sc, glassSphere);

    _initMode(MODE_VGA_13H);

    _waitvretrace();

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            vec3 scaledPixelColor;
            color pixelColor = {0, 0, 0};
            
            for (i = 0; i < SAMPLES_PER_PIXEL; ++i) {
                struct Ray r = cam->getRay(cam, x, y);
                vec3 rayCol = rayColor(&r, MAX_DEPTH, sc);
                pixelColor = v3Add(&pixelColor, &rayCol);
            }
            
            scaledPixelColor = v3MultiplyN(&pixelColor, PIXEL_SAMPLES_SCALE);
            _putpixel(x, y, pixel2vga(&scaledPixelColor));
        }
    }

    getch();
    sc->clear(sc);
    free(sc);
    free(cam);
    _initMode(MODE_VGA_3H);
}
