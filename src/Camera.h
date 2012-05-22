#ifndef CAMERA_H
#define CAMERA_H
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Math/Ray.h"
#include "Image.h"
#include <QList>
#include "Scene.h"
#include "RayTracer.h"

class PhotonMap;

/**
  Camera class
  */
class Camera
{
public:
    Camera();
    /**
      Creates camera with given position and target point, near and far plane and fov angle
      */
    Camera(Vector3 positon, Vector3 target, float nearPlane, float farPlane, float fov);

    /**
      Returns image to which camera renders
      */
    Image* GetImage() const {
        return img;
    }

    /**
      Sets image to which camera will render
      */
    void SetImage(Image* image) {
        img = image;
    }

    /**
      Returns position of camera
      */
    Vector3 GetPosition() const {
        return position;
    }

    /**
      Sets position of camera
      */
    void SetPosition(Vector3 p) {
        position = p;
    }

    /**
      Returns target point of camera
      */
    Vector3 GetTarget() const {
        return target;
    }

    /**
      Sets target point of camera
      */
    void SetTarget(Vector3 t) {
        target = t;
    }

    /**
      Returns near plane of camera
      */
    float GetNearPlane() const {
        return nearPlane;
    }

    /**
      Sets near plane of camera
      */
    void SetNearPlane(float np) {
        nearPlane = np;
    }

    /**
      Returns far plane of camera
      */
    float GetFarPlane() const {
        return farPlane;
    }

    /**
      Set far plane of camera
      */
    void SetFarPlane(float fp) {
        farPlane = fp;
    }

    /** Renders scene without use of photon mapping
        \param scene scene
        \param numSamples number of samples per pixel
    */
    void RenderScene(Scene* scene, unsigned int numSamples=1);

    /** Renders scene with photon mapping
        \param scene scene
        \param numSamples number of samples per pixel
        \param numGlobalMapPhotons number of global map photons
        \param numCausticMapPhotons number of caustic map photons
    */
    void RenderScene(Scene* scene, unsigned int numSamples, unsigned int numGlobalMapPhotons,
                     unsigned int numCausticMapPhotons);

    /** Renders scene with streamed photon mapping
        \param scene scene
        \param numSamples number of samples per pixel
        \param numGlobalMapPhotons number of global map photons
        \param numCausticMapPhotons number of caustic map photons
        \param numAssociatedPhotons number of associated photons
        \param radius radius of photons stream
    */
    void RenderSceneStream(Scene* scene, unsigned int ns, unsigned int numGlobalMapPhotons, unsigned int numCausticMapPhotons, int numAssociatedPhotons, float radius);

    void VisualizePhotonMap(Scene* scene, int numPhotons, int maxReflections);
    void VisualizeStreamPhotonMap(Scene* scene, int numPhotons, int maxReflections, int numAssociatedPhotons, float radius);

    void SetResultFileName(const char* filename) {
        renderFileName = QString(filename);
    }

    void SetPhotonMappingParams(float globalMapRadius, int globalMapNumPhotons,
                                float causticMapRadius, int causticMapNumPhotons) {
        rayTracer.SetPhotonMappingParams(globalMapRadius, globalMapNumPhotons,
                                         causticMapRadius, causticMapNumPhotons);
    }

private:
    Vector3 position;
    Vector3 target;

    float nearPlane;
    float farPlane;
    float fov;
    Image* img;

    float distance;

    Matrix4x4 projectionMatrix;
    Matrix4x4 viewMatrix;
    Matrix4x4 vpMatrix;
    Matrix4x4 invVPMatrix;

    RayTracer rayTracer;

    void Recalculate();

    QString renderFileName;
};

#endif // CAMERA_H
