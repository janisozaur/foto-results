#ifndef AREALIGHT_H
#define AREALIGHT_H
#include "AmbientLight.h"
#include "Math/Vector2.h"
#include <QVector>
#include "PointLight.h"

class AreaLight : public AmbientLight
{
public:
    /**
      Creates array of point lights, with given center and size.
      All lights in array have the same color and attenuation
      */
    AreaLight(Vector3 center, Vector2 size, Color color, Vector3 attenuation, int density);
    ~AreaLight();

    LightIntensity GetLightIntensity(Vector3 cameraPosition, IntersectionResult *ir, QList<Geometry *> &geometry);
    Ray GetPhoton(bool useProjectionMap=false) const;
    void CreateProjectionMap(const Scene* scene);
    float GetProjectionMapRatio() const;

protected:
    bool IsInShadow(IntersectionResult *ir, QList<Geometry *> &geometry);
private:
    QVector<PointLight*> lights;
    QVector<bool> inShadows;
    Vector3 center;
    Vector2 size;
};

#endif // AREALIGHT_H
