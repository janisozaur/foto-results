#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "LightIntensity.h"
#include "Math/Ray.h"

class Scene;
class PhotonMap;
class StreamPhotonMap;

/**
  Traces ray when rendering scene
  */
class RayTracer
{
public:
    RayTracer();

    /**
      Traces ray with use of photon map.
      \param ray ray to trace
      \param scene scene
      \param cameraPosition position of camera
      \param maxReflections maximum number of ray reflections
      \param exposure exposure of colors
      \param globalMap global photon map
      \param causticMap caustic photon map
      */
    LightIntensity TraceRay(const Ray&ray, Scene*scene, const Vector3 cameraPosition,
                            int maxReflections, float exposure=1,
                            PhotonMap*globalMap=0, PhotonMap* causticMap=0);

    /**
      Traces ray with use of streamed photon map.
      \param ray ray to trace
      \param scene scene
      \param cameraPosition position of camera
      \param maxReflections maximum number of ray reflections
      \param exposure exposure of colors
      \param globalMap global streamed photon map
      \param causticMap caustic streamed photon map
      */
    LightIntensity TraceRayStream(const Ray&ray, Scene*scene, const Vector3 cameraPosition,
                            int maxReflections, float exposure,
                            StreamPhotonMap*globalMap,
                            StreamPhotonMap*causticMap);

    void SetPhotonMappingParams(float globalRadius, int globalNumPhotons,
                                float causticRadius, int causticNumPhotons);

private:
    float globalRadius;
    float causticRadius;
    int globalNumPhotons;
    int causticNumPhotons;
};

#endif // RAYTRACER_H
