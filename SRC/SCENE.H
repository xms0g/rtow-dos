#ifndef SCENE_H
#define SCENE_H

#include "bool.h"

struct Ray;
struct HitRecord;
struct Sphere;

void scInit(int size);

void scAdd(const struct Sphere* object);

void scClear();

bool scHit(const struct Ray* ray, double tmin, double tmax, struct HitRecord* rec);

#endif