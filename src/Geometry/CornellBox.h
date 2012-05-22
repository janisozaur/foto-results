#ifndef CORNELLBOX_H
#define CORNELLBOX_H
#include "Geometry.h"
#include "Triangle.h"
#include "Material.h"

/**
  Constructs Cornell box
  */
class CornellBox : public Geometry
{
public:
    /**
      Constructs cornell box with center pos and sizes in each axis defined by size vector
      */
    CornellBox(Vector3 pos, Vector3 size);
    ~CornellBox();

    IntersectionResult Intersects(const Ray &ray, float range) const;

private:
    Material* redWallMaterial;
    Material* greenWallMaterial;
    Material* whiteWallMaterial;

    QList<Triangle*> walls;
};

#endif // CORNELLBOX_H
