#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Geometry.h"

/**
  Triangle geometry class
  */
class Triangle : public Geometry
{
public:
    Triangle();
    /**
      Construct triangle based on three vertices.
      Normal is calculated as cross product of these vertices and assigned to all of them.
      */
    Triangle(const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3);

    /**
      Construct triangle based on three vertices and normals assigned to them.
      */
    Triangle(const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3,
             const Vector3& normal1, const Vector3& normal2, const Vector3& normal3);

    Vector3 vertices[3];
    Vector3 normals[3];

    IntersectionResult Intersects(const Ray& ray, float range=0.0f) const;
};

#endif // TRIANGLE_H
