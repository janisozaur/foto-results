#ifndef SCENE_H
#define SCENE_H
#include "Lights/AmbientLight.h"
#include "Geometry/Geometry.h"


/**
  Represents scene that we want to render.
  Contains all geometry and light.
  */
class Scene
{
public:
    Scene();
    ~Scene();

    /**
      Adds geometry to scene
      */
    void AddGeometry(Geometry*g) {
        geometry.append(g);
    }

    /**
      Adds light to scene
      */
    void AddLight(AmbientLight* l) {
        lights.append(l);
    }

    QList<Geometry*> geometry;
    QList<AmbientLight*> lights;
};

#endif // SCENE_H
