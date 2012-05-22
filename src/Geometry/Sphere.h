#ifndef SPHERE_H
#define SPHERE_H
#include "Geometry.h"

/**
  Sphere geometry
  */
class Sphere : public Geometry
{
public:
    Sphere() : Geometry(){}
    /**
      Constructs sphere with center pos and radius r.
      \param pos position of center of sphere
      \param r radius of sphere
      */
    Sphere(Vector3 pos, float r) : Geometry(), position(pos), radius(r) {}

    Vector3 position;
    float radius;

    IntersectionResult Intersects(const Ray& ray, float range=0.0f) const;
    Vector3 MapToLocal(Vector3 &vec) const;
};

QDebug operator<<(QDebug dbg, const Sphere &s);

#endif // SPHERE_H
