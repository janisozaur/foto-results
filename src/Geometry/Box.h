#ifndef BOX_H
#define BOX_H

#include "Geometry.h"
#include "Triangle.h"
#include "Material.h"

/**
  Box geometry
  */
class Box : public Geometry
{
public:
    /**
      Constructs box with center at pos, and sizes in each axis defined by size vector
      */
    Box(Vector3 pos, Vector3 size);
    ~Box();

    IntersectionResult Intersects(const Ray &ray, float range) const;
    void SetMaterial(Material *m);

private:
    QList<Triangle*> walls;
};

#endif // BOX_H
