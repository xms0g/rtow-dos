#include "camera.h"
#include "math.h"

static vec3 sampleSquare();
static vec3 camDefocusDiskSample(const Camera* cam);

void camInit(Camera* cam, double aspectRatio, double vfov, double defocusAngle, double focusDist, int imageWidth, const vec3* lookfrom, const vec3* lookat, const vec3* vup) {
    double theta, h, viewportHeight, viewportWidth, defocusRadius;
    vec3 subLookFromAt, w, v3CrossVupW, u, v, viewport_u, viewport_v, addUV, halfAddUV, mulWFocusDist, viewportUpperLeft, pixel00Offset;
    
    int imageHeight = (int)(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    theta = degree2radian(vfov);
    h = tan(theta/2);
    viewportHeight = 2.0 * h * focusDist;
    viewportWidth = viewportHeight * ((double)imageWidth/imageHeight);

    cam->center = *lookfrom;
    cam->defocusAngle = defocusAngle;

    // Calculate the u,v,w v3Unit basis vectors for the camera coordinate frame.
    subLookFromAt = v3Subtract(lookfrom, lookat);
    w = v3Unit(&subLookFromAt);
    v3CrossVupW = v3Cross(vup, &w);
    u = v3Unit(&v3CrossVupW);
    v = v3Cross(&w, &u);

    // Calculate the vectors av3Cross the horizontal and down the vertical viewport edges.
    viewport_u = v3MultiplyN(&u, viewportWidth);
    viewport_v = v3MultiplyN(&v, -viewportHeight);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    cam->pixelDeltaU = v3DivideN(&viewport_u, imageWidth);
    cam->pixelDeltaV = v3DivideN(&viewport_v, imageHeight);

    // Calculate the location of the upper left pixel.
    addUV = v3Add(&viewport_u, &viewport_v);
    halfAddUV = v3MultiplyN(&addUV, 0.5);
    mulWFocusDist = v3MultiplyN(&w, focusDist);
    viewportUpperLeft = v3Subtract(&cam->center, &mulWFocusDist);
    viewportUpperLeft = v3Subtract(&viewportUpperLeft, &halfAddUV);
    pixel00Offset = v3Add(&cam->pixelDeltaU, &cam->pixelDeltaV);
    pixel00Offset = v3MultiplyN(&pixel00Offset, 0.5);
    cam->pixel00Loc = v3Add(&viewportUpperLeft, &pixel00Offset);

    // Calculate the camera defocus disk basis vectors.
    defocusRadius = focusDist * tan(degree2radian(defocusAngle / 2));
    cam->defocusDiskU = v3MultiplyN(&u, defocusRadius);
    cam->defocusDiskV = v3MultiplyN(&v, defocusRadius);
}

Ray getRay(const Camera* cam, int x, int y) {
    Ray result;
    vec3 rayOrig, rayDir;
    vec3 offset = sampleSquare();
    vec3 pixDeltaUMult = v3MultiplyN(&cam->pixelDeltaU, x + offset.x);
    vec3 pixDeltaVMult = v3MultiplyN(&cam->pixelDeltaV, y + offset.y);
    vec3 sumPixDeltas = v3Add(&pixDeltaUMult, &pixDeltaVMult);
    vec3 pixelSample = v3Add(&cam->pixel00Loc, &sumPixDeltas);

    rayOrig = (cam->defocusAngle <= 0) ? cam->center : camDefocusDiskSample(cam);
    rayDir = v3Subtract(&pixelSample, &rayOrig);
    
    result.origin = rayOrig;
    result.direction = rayDir;
    
    return result;
}

vec3 sampleSquare() {
    vec3 result;
    
    result.x = randd() - 0.5;
    result.y = randd() - 0.5;
    result.z = 0;
    
    return result;
}

vec3 camDefocusDiskSample(const Camera* cam) {
    // Returns a random point in the camera defocus disk.
    vec3 p = v3RandomInUnitDisk();
    vec3 defocusU = v3MultiplyN(&cam->defocusDiskU, p.x);
    vec3 defocusV = v3MultiplyN(&cam->defocusDiskV, p.y);
    vec3 diskSample = v3Add(&defocusU, &defocusV);

    return v3Add(&cam->center, &diskSample);
}