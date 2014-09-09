#ifndef SCENESPHERE_H
#define SCENESPHERE_H

#include "Scene.h"
#include "Geometry/Cube.h"
#include "Geometry/Sphere.h"
#include "SPH/SPH.h"

class SceneSphere : public Scene
{
public:
    SceneSphere();
    virtual ~SceneSphere();

    virtual SPH& sph();

private:
    Cube _sky;
    Sphere _sphere;
    SPH _water;
};

#endif // SCENESPHERE_H
