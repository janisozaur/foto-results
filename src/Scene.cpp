#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene() {
    qDeleteAll(geometry);
    qDeleteAll(lights);
    geometry.clear();
    lights.clear();
}
