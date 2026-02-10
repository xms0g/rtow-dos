#include "camera.h"
#include "math.h"

Camera::Camera(double aspectRatio, double vfov, double defocusAngle, double focusDist, int imageWidth, const vec3* lookfrom, const vec3* lookat, const vec3* vup) {
    int imageHeight = (int)(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    
    mCenter = *lookfrom;
    mDefocusAngle = defocusAngle;
    
    double theta = degree2radian(vfov);
    double h = tan(theta/2);
    double viewportHeight = 2.0 * h * focusDist;
    double viewportWidth = viewportHeight * ((double)imageWidth/imageHeight);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    vec3 subLookFromAt = v3Subtract(lookfrom, lookat);
    vec3 w = unit(&subLookFromAt);
    vec3 crossVupW = cross(vup, &w);
    vec3 u = unit(&crossVupW);
    vec3 v = cross(&w, &u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = v3MultiplyN(&u, viewportWidth);
    vec3 viewport_v = v3MultiplyN(&v, -viewportHeight);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    mPixelDeltaU = v3DivideN(&viewport_u, imageWidth);
    mPixelDeltaV = v3DivideN(&viewport_v, imageHeight);

    // Calculate the location of the upper left pixel.
    vec3 addUV = v3Add(&viewport_u, &viewport_v);
    vec3 halfAddUV = v3MultiplyN(&addUV, 0.5);
    vec3 mulWFocusDist = v3MultiplyN(&w, focusDist);
    vec3 viewportUpperLeft = v3Subtract(&mCenter, &mulWFocusDist);
    viewportUpperLeft = v3Subtract(&viewportUpperLeft, &halfAddUV);
    vec3 pixel00Offset = v3Add(&mPixelDeltaU, &mPixelDeltaV);
    pixel00Offset = v3MultiplyN(&pixel00Offset, 0.5);
    mPixel00Loc = v3Add(&viewportUpperLeft, &pixel00Offset);

    // Calculate the camera defocus disk basis vectors.
    double defocusRadius = focusDist * tan(degree2radian(defocusAngle / 2));
    mDefocusDiskU = v3MultiplyN(&u, defocusRadius);
    mDefocusDiskV = v3MultiplyN(&v, defocusRadius);
}

struct Ray Camera::getRay(int x, int y) const {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location x, y.

    vec3 offset = sampleSquare();
    vec3 pixDeltaUMult = v3MultiplyN(&mPixelDeltaU, x + offset.x);
    vec3 pixDeltaVMult = v3MultiplyN(&mPixelDeltaV, y + offset.y);
    vec3 sumPixDeltas = v3Add(&pixDeltaUMult, &pixDeltaVMult);
    vec3 pixelSample = v3Add(&mPixel00Loc, &sumPixDeltas);

    vec3 rayOrig = (mDefocusAngle <= 0) ? mCenter : defocusDiskSample();;
    vec3 rayDir = v3Subtract(&pixelSample, &rayOrig);
    
    struct Ray result;
    result.origin = rayOrig;
    result.direction = rayDir;
    return result;
}

vec3 Camera::sampleSquare() const {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    vec3 result = {randd() - 0.5, randd() - 0.5, 0};

    return result;
}

vec3 Camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk.
    vec3 p = randomInUnitDisk();
    vec3 defocusU = v3MultiplyN(&mDefocusDiskU, p.x);
    vec3 defocusV = v3MultiplyN(&mDefocusDiskV, p.y);
    vec3 diskSample = v3Add(&defocusU, &defocusV);

    return v3Add(&mCenter, &diskSample);
}