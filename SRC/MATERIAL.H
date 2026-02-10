#ifndef MATERIAL_H
#define MATERIAL_H

#include "sceneobj.h"
#include "color.h"
#include "math.h"

class Material {
public:
    virtual ~Material() {}

    virtual bool scatter(const struct Ray* in, const HitRecord* rec, color* attenuation, struct Ray* scattered) const {
        (void)in;
        (void)rec;
        (void)attenuation;
        (void)scattered;
        return false;
    }
};

class Lambertian: public Material {
public:
    Lambertian(const color* albedo) : albedo(*albedo) {}

    virtual bool scatter(const struct Ray* in, const HitRecord* rec, color* attenuation, struct Ray* scattered) const {
        (void)in;
        vec3 randomUnitVec = v3RandomUnitVec();
        vec3 scatterDir = v3Add(&rec->normal, &randomUnitVec);
        // Catch degenerate scatter direction
        if (nearZero(&scatterDir)) {
            scatterDir = rec->normal;
        }
        
        scattered->origin = rec->p;
        scattered->direction = scatterDir;
        *attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class Metal: public Material {
public:
    Metal(const color* albedo, double fuzz) : albedo(*albedo), mFuzz(fuzz < 1 ? fuzz: 1) {}

    virtual bool scatter(const struct Ray* in, const HitRecord* rec, color* attenuation, struct Ray* scattered) const {
        vec3 reflected = reflect(rayDirection(in), &rec->normal);
        vec3 randomUnitVec = v3RandomUnitVec();
        vec3 reflectedUnit = unit(&reflected);
        vec3 fuzzVec = v3MultiplyN(&randomUnitVec, mFuzz);
        reflected = v3Add(&reflectedUnit, &fuzzVec);
        
        scattered->origin = rec->p;
        scattered->direction = reflected;
        
        *attenuation = albedo;
        return (bool)(dot(rayDirection(scattered), &rec->normal) > 0);
    }

private:
    color albedo;
    double mFuzz;
};

class Dielectric: public Material {
public:
    Dielectric(double refractionIndex) : mRefractionIndex(refractionIndex) {}

    virtual bool scatter(const struct Ray* in, const HitRecord* rec, color* attenuation, struct Ray* scattered) const {
        // the glass surface absorbs nothing.
        attenuation->x = 1.0;
        attenuation->y = 1.0;
        attenuation->z = 1.0;
        
        double ri = rec->frontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;
        vec3 unitDir = unit(rayDirection(in));
        vec3 v3Negated = v3Negate(&unitDir);
        double cosTheta = fmin(dot(&v3Negated, &rec->normal), 1.0);
        double invSinTheta = invSqrt(1.0 - cosTheta * cosTheta);
        bool cannotRefract = (bool)(ri / invSinTheta > 1.0);

        vec3 dir;
        if (cannotRefract || reflectance(cosTheta, ri) > randd()) {
            dir = reflect(&unitDir, &rec->normal);
        } else {
            dir = refract(&unitDir, &rec->normal, ri);
        }

        scattered->origin = rec->p;
        scattered->direction = dir;

        return true;
    }

private:
    double mRefractionIndex;

    double reflectance(double cosine, double refractionIndex) const {
        // Use Schlick's approximation for reflectance.
        double r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif