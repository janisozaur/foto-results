#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include "Math/Vector3.h"
#include "LightIntensity.h"
#include <QList>
#include "Lights/AmbientLight.h"
#include "Scene.h"
#include "KDTree.h"


//struct describing photon
struct Photon {
    Vector3 position;
    LightIntensity energy;
    Vector3 direction;
    unsigned char divisionAxis;
};




class PhotonMap
{
public:
    PhotonMap();
    ~PhotonMap();

    /**
      Generates photon map
      \param scene scene to generate photon map
      \param numPhotons number of photons to be emmited
      \param maxReflections maximum number of photon reflections
      \param caustic indicates if generated photon map should be caustic photon map
      */
    void GeneratePhotonMap(Scene* scene, int numPhotons, int maxReflections, bool caustic=false);

    /**
      Returns list of closest photons to given point in given radius.
      \param point point of interest
      \param radius radius of search
      \param maxPhotons maximum number of photons to find
      */
    QList<Photon*> GetClosestPhotons(Vector3 point, float radius, int maxPhotons);

private:
    //generates photons from single light
    void GeneratePhotons(AmbientLight* light, QList<Geometry*>* geometry, int numPhotons, bool caustic, int maxReflections);

    //trace photon in scene and saves informatino to photon map
    void TracePhoton(LightIntensity photonEnergy, const Ray &startRay, QList<Geometry *> *geometry,
                     QList<Photon *> *photons, int reflections);
    QList<Photon*> photons;
    KDTree<Photon>* kdTree;

    int maxPhotons;

    float PropabilityOfAbsorption();
    Vector3 LambertReflectionDirection(const IntersectionResult& ir);
};

#endif // PHOTONMAP_H
