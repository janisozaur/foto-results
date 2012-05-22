#include "StreamPhotonMap.h"
#include <cfloat>
#include "KDTree.h"
#include "Lights/AreaLight.h"

#define BIAS 0.001f

const float RADIUS=2.5;

StreamPhotonMap::StreamPhotonMap() {
    maxPhotons=5000000;
    kdTree = new KDTree<Stream>();
}

StreamPhotonMap::~StreamPhotonMap() {
    qDeleteAll(photons);
    photons.clear();

    delete kdTree;
}


void StreamPhotonMap::GeneratePhotonMap(Scene *scene, int numPhotons, int maxReflections, bool caustic) {
    int numPointLights=0;
    //count how much points lights are in scene
    for(int i=0;i<scene->lights.count();i++) {
        if(scene->lights.at(i)->type==POINT || scene->lights.at(i)->type == AREA) {
            numPointLights++;
            if(caustic) {
                //if we generate caustic map, we must generate first projection map for each light
                scene->lights.at(i)->CreateProjectionMap(scene);
            }
        }
    }

    //adjust number of photons for each light
    numPhotons/=numPointLights;

    for(int i=0;i<scene->lights.count();i++) {
        if(scene->lights.at(i)->type==POINT || scene->lights.at(i)->type==AREA)
            //generate photons for each light in scene
            GeneratePhotons(scene->lights.at(i), &scene->geometry, numPhotons, caustic, maxReflections);
    }


    //build kdtree from photons
    kdTree->Build(photons);
    photons.clear();
}

//generate streams for given light
void StreamPhotonMap::GeneratePhotons(AmbientLight *light, QList<Geometry*>* geometry, int numPhotons, bool caustic, int maxReflections) {

    QList<Stream*> tempPhotons;

    while(tempPhotons.count()<numPhotons && photons.count()+tempPhotons.count()<maxPhotons) {
        Ray photonRay;

        //generate lead photon
        photonRay = light->GetPhoton(caustic);

        //add associated photons.
        //At begining all associated photons have the same position as lead photon
        Stream startStream;
        for(int j=0;j<numAssociatedPhotons;j++) {
            SinglePhoton sp;
            sp.position = photonRay.origin;
            startStream.associatedPhoton.push_back(sp);
        }

        TracePhoton(light->color, photonRay, geometry, &tempPhotons, &startStream, maxReflections);
    }

    float scale;
    if(caustic) {
        if(light->type == POINT || light->type == AREA)
            scale = (1.0f/tempPhotons.count())*light->GetProjectionMapRatio();
    }
    else
        scale = 1.0f/tempPhotons.count();

    for(int i=0;i<tempPhotons.count();i++) {

        tempPhotons[i]->leadingPhoton.energy*=scale;
        for(int j=0;j<tempPhotons[i]->associatedPhoton.count();j++)
            tempPhotons[i]->associatedPhoton[j].energy*=scale;
    }

    photons.append(tempPhotons);
}

//traces photon stream
void StreamPhotonMap::TracePhoton(LightIntensity photonEnergy, const Ray &startRay, QList<Geometry *> *geometry,
                            QList<Stream *> *photons, Stream* parent, int reflections) {

    int closest=-1;
    float closestDist=FLT_MAX;
    IntersectionResult closestIntersection;

    IntersectionResult result;
    //check intersection of lead photon with geometry in scene
    for(int j=0;j<geometry->count();j++) {
        result = geometry->at(j)->Intersects(startRay);
        if(result.type!=MISS) {
            if(closestDist>result.distance) {
                closestDist = result.distance;
                closest = j;
                closestIntersection = result;
            }
        }
    }
    if(closest!=-1) {
        //if photon intersects with reflective object and we can still reflect photon
        if(closestIntersection.object->GetMaterial()->type==REFLECTIVE && reflections>0) {
            Vector3 reflected = startRay.direction.Reflect(closestIntersection.intersectionPointNormal);
            reflected.Normalize();
            Ray newRay(closestIntersection.point+reflected*BIAS, reflected);

            //temporary parent stream with no associated photons
            Stream tempStream;


            TracePhoton(photonEnergy, newRay, geometry, photons, &tempStream, reflections-1);
        }
        else if(closestIntersection.object->GetMaterial()->type==REFRACTIVE && reflections>0) {
            RefractiveMaterial* mat = (RefractiveMaterial*)closestIntersection.object->GetMaterial();

            Vector3 refracted;
            if(closestIntersection.type==HIT)
                refracted = startRay.direction.Refract(closestIntersection.intersectionPointNormal,
                                                  mat->etaRate);
            else
                refracted = startRay.direction.Refract(-closestIntersection.intersectionPointNormal,
                                                  1.0f/mat->etaRate);
            refracted.Normalize();

            Ray newRay(closestIntersection.point+refracted*BIAS, refracted);

            //temporary parent stream with no associated photons
            Stream tempStream;


            TracePhoton(photonEnergy, newRay, geometry, photons, &tempStream, reflections-1);
        }
        else {
            //we hit diffuse geometry

            DiffuseMaterial* mat = (DiffuseMaterial*)closestIntersection.object->GetMaterial();

            //new photon stream to store in stream map
            Stream* newStream = new Stream;
            newStream->leadingPhoton.energy = photonEnergy;
            newStream->leadingPhoton.position = closestIntersection.point;
            newStream->position = closestIntersection.point;
            newStream->leadingPhoton.direction = -startRay.direction;

            //generate new associated photons
            for(int j=0;j<parent->associatedPhoton.count();j++) {
                float x,y,z;
                do {
                    x = 2.0f*((float)qrand())/RAND_MAX-1.0f;
                    y = 2.0f*((float)qrand())/RAND_MAX-1.0f;
                    z = 2.0f*((float)qrand())/RAND_MAX-1.0f;
                } while(x*x+y*y+z*z>1);

                Vector3 newAssociatedPos(x,y,z);
                newAssociatedPos*=radius;
                newAssociatedPos += closestIntersection.point;

                //check if ray from old associated photon to new intersects something
                Ray oldAssociatedToNew(parent->associatedPhoton[j].position, newAssociatedPos - parent->associatedPhoton[j].position);
                IntersectionResult iresult;
                for(int k=0;k<geometry->count();k++) {
                    iresult = geometry->at(k)->Intersects(oldAssociatedToNew, (newAssociatedPos - parent->associatedPhoton[j].position).GetLength());
                    if(iresult.type!=MISS) {
                       break;
                    }
                }

                if(iresult.type!=MISS)
                    continue;

                //if there are no intersection, we add new associated photon to stream
                SinglePhoton newAssociated;
                newAssociated.position = newAssociatedPos;
                newAssociated.direction = -oldAssociatedToNew.direction;
                newAssociated.energy = photonEnergy;
                newStream->associatedPhoton.push_back(newAssociated);
            }

            if(newStream->associatedPhoton.count())
                newStream->leadingPhoton.energy = photonEnergy/newStream->associatedPhoton.count();
            for(int j=0;j<newStream->associatedPhoton.count();j++)
                newStream->associatedPhoton[j].energy = photonEnergy/newStream->associatedPhoton.count();

            photons->append(newStream);

            if(PropabilityOfAbsorption()<mat->absorptionPropability && reflections>0) {
                float avgDiff = (mat->diffuse.r+mat->diffuse.g+mat->diffuse.b)/3;
                photonEnergy *= (mat->diffuse/avgDiff);
                Vector3 reflected = LambertReflectionDirection(closestIntersection);
                Ray newRay(closestIntersection.point+reflected*BIAS, reflected);
                TracePhoton(photonEnergy, newRay, geometry, photons, newStream, reflections-1);
            }
        }
    }
}

float StreamPhotonMap::PropabilityOfAbsorption() {
    return ((float)qrand())/RAND_MAX;
}

Vector3 StreamPhotonMap::LambertReflectionDirection(const IntersectionResult &ir) {

    float x,y,z;
    do {
        x = 2.0f*((float)qrand())/RAND_MAX-1.0f;
        y = 2.0f*((float)qrand())/RAND_MAX-1.0f;
        z = 2.0f*((float)qrand())/RAND_MAX-1.0f;
    } while(x*x+y*y+z*z>1);

    Vector3 direction(x,y,z);

    if(direction.DotProduct(ir.intersectionPointNormal)<0)
        direction*=-1;

    return direction;
}

QList<Stream*> StreamPhotonMap::GetClosestPhotons(Vector3 point, float radius, int maxPhotons) {
    return kdTree->FindClosest(point, radius, maxPhotons);
}
