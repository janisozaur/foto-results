#ifndef STREAMPHOTONMAP_H
#define STREAMPHOTONMAP_H

#include "Math/Vector3.h"
#include "LightIntensity.h"
#include <QList>
#include "Lights/AmbientLight.h"
#include "Scene.h"
#include "KDTree.h"


struct SinglePhoton {
    Vector3 position;
    LightIntensity energy;
    Vector3 direction;
};

struct Stream {
    //due to kdtree template;
    Vector3 position;
    SinglePhoton leadingPhoton;
    QVector<SinglePhoton> associatedPhoton;
    unsigned char divisionAxis;
};

class StreamPhotonMap
{
public:
    StreamPhotonMap();
    ~StreamPhotonMap();

    /**
      Generates streams map
      \param scene scene to generate photon map
      \param numPhotons number of photons to be emmited
      \param maxReflections maximum number of photon reflections
      \param caustic indicates if generated photon map should be caustic photon map
      */
    void GeneratePhotonMap(Scene* scene, int numPhotons, int maxReflections, bool caustic=false);

    /**
      Returns list of closest streams to given point in given radius.
      \param point point of interest
      \param radius radius of search
      \param maxPhotons maximum number of streams to find
      */
    QList<Stream*> GetClosestPhotons(Vector3 point, float radius, int maxPhotons);

    /**
      Sets radius of stream
      */
    void SetRadius(float radius) {
        this->radius = radius;
    }

    /**
      Sets starting number of associated photons in stream
      */
    void SetNumAssociatedPhotons(int n) {
        numAssociatedPhotons = n;
    }

private:
    void GeneratePhotons(AmbientLight* light, QList<Geometry*>* geometry, int numPhotons, bool caustic, int maxReflections);
    void TracePhoton(LightIntensity photonEnergy, const Ray &startRay, QList<Geometry *> *geometry,
                     QList<Stream *> *photons, Stream* parent, int reflections);
    QList<Stream*> photons;
    KDTree<Stream>* kdTree;

    int maxPhotons;

    float PropabilityOfAbsorption();
    Vector3 LambertReflectionDirection(const IntersectionResult& ir);

    float radius;
    int numAssociatedPhotons;
};

#endif // STREAMPHOTONMAP_H
