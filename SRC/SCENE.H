#ifndef SCENE_H
#define SCENE_H

#include "sceneobj.h"
#include "vector.h"

class Scene: public SceneObject {
public:
    Scene() {}

    Scene(SceneObject* object) { add(object); }
    
    ~Scene();
    
    void clear() { objects.clear(); }

    void add(SceneObject* object) { objects.push_back(object); }

    virtual bool hit(const Ray* ray, double tmin, double tmax, HitRecord* rec) const;

private:
    vector<SceneObject*> objects;
};

#endif