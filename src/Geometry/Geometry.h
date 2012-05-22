#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "Math/Ray.h"
#include "LightIntensity.h"
#include "Material.h"

/**
    Base class that represents geometry.
    Contains method to check if it intersects given ray.

  */
class Geometry {
public:
    Geometry() {material=0;}
    virtual ~Geometry(){}

    /**
      Checks if this geometry is intersected by ray in given range
      \param ray intersection ray
      \param range range in which intersection have to occured to be counted as intersection
      */
    virtual IntersectionResult Intersects(const Ray& ray, float range=0.0f) const=0;

    /**
      Sets material to geometry
      */
    virtual void SetMaterial(Material* m) {
        material = m;
    }

    /**
      Returns material of geometry
      */
    Material* GetMaterial() const {
        return material;
    }

    /**
      Maps given vector to local vector in coordinantes of geometry.
      Used in spherical mapping
      */
    virtual Vector3 MapToLocal(Vector3& vec) const {
        return vec;
    }

protected:
    Material* material;
};


#endif // GEOMETRY_H
