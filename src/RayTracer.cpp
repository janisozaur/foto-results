#include "RayTracer.h"
#include "Scene.h"
#include "PhotonMap.h"
#include <cfloat>
#include <algorithm>
#include "StreamPhotonMap.h"
using namespace std;

#define BIAS 0.001f


RayTracer::RayTracer() {
    globalRadius = 5;
    causticRadius = 5;
    globalNumPhotons = 400;
    causticNumPhotons = 100;
}

void RayTracer::SetPhotonMappingParams(float globalRadius, int globalNumPhotons,
                                       float causticRadius, int causticNumPhotons) {
    this->globalNumPhotons = globalNumPhotons;
    this->globalRadius = globalRadius;
    this->causticNumPhotons = causticNumPhotons;
    this->causticRadius = causticRadius;

}

LightIntensity RayTracer::TraceRay(const Ray&ray, Scene*scene, const Vector3 cameraPosition,
                                   int reflections, float exposure,
                                   PhotonMap* globalMap, PhotonMap* causticMap) {
    LightIntensity resultIntensity;

    int closest=-1;
    float closestDist=FLT_MAX;
    IntersectionResult closestIntersection;

    IntersectionResult result;
    for(int j=0;j<scene->geometry.count();j++) {
        result = scene->geometry.at(j)->Intersects(ray);
        if(result.type!=MISS) {
            if(closestDist>result.distance) {
                closestDist = result.distance;
                closest = j;
                closestIntersection = result;
            }
        }
    }
    if(closest!=-1) {
        if(closestIntersection.object->GetMaterial()->type==REFLECTIVE && reflections>0) {
            Vector3 reflected = ray.direction.Reflect(closestIntersection.intersectionPointNormal);
            reflected.Normalize();
            Ray newRay(closestIntersection.point+reflected*BIAS, reflected);

            resultIntensity += TraceRay(newRay, scene, cameraPosition, reflections-1, exposure, globalMap, causticMap);
        }
        else if(closestIntersection.object->GetMaterial()->type==REFRACTIVE && reflections>0) {
            float reflectionCoef = max(0.0, min(1.0, 0.05+0.11*(pow(1+ray.direction.DotProduct(closestIntersection.intersectionPointNormal), 1))));
            LightIntensity reflectedIntensity;
            LightIntensity refractedIntensity;

            RefractiveMaterial* mat = (RefractiveMaterial*)closestIntersection.object->GetMaterial();

            Vector3 refracted;
            if(closestIntersection.type==HIT)
                refracted = ray.direction.Refract(closestIntersection.intersectionPointNormal,
                                                  mat->etaRate);
            else
                refracted = ray.direction.Refract(-closestIntersection.intersectionPointNormal,
                                                  1.0f/mat->etaRate);
            refracted.Normalize();

            Ray newRay(closestIntersection.point+refracted*BIAS, refracted);

            refractedIntensity += TraceRay(newRay, scene, cameraPosition, reflections-1, exposure, globalMap, causticMap);

            Vector3 reflected = ray.direction.Reflect(closestIntersection.intersectionPointNormal);
            reflected.Normalize();
            newRay = Ray(closestIntersection.point+reflected*BIAS, reflected);

            reflectedIntensity += TraceRay(newRay, scene, cameraPosition, reflections-1, exposure, globalMap, causticMap);


            resultIntensity = reflectionCoef*reflectedIntensity + (1-reflectionCoef)*refractedIntensity;
        }
        else {
            LightIntensity fromLights;
            for(int j=0;j<scene->lights.count();j++) {
                fromLights += scene->lights.at(j)->GetLightIntensity(cameraPosition, &closestIntersection, scene->geometry);
            }
            resultIntensity += fromLights;

            if(globalMap) {
                QList<Photon*> closest = globalMap->GetClosestPhotons(closestIntersection.point, globalRadius, globalNumPhotons);

                if(closest.count()!=0) {
                    float r = (closest[0]->position-closestIntersection.point).GetLength();

                    LightIntensity E;
                    for(int z=0;z<closest.count();z++) {
                        float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->direction);
                        if(dot>0) {
                            E += exposure*closest[z]->energy*dot;
                        }
                    }
                   resultIntensity += (E/(M_PI*r*r));
                }
            }

            if(causticMap) {
                QList<Photon*> closest = causticMap->GetClosestPhotons(closestIntersection.point, causticRadius, causticNumPhotons);

                if(closest.count()!=0) {
                    float r = (closest[0]->position-closestIntersection.point).GetLength();

                    LightIntensity E;
                    for(int z=0;z<closest.count();z++) {
                        float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->direction);
                        if(dot>0) {
                            E += exposure*closest[z]->energy*dot;
                        }
                    }
                   resultIntensity += (E/(M_PI*r*r));
                }
            }

        }
        if(closestIntersection.object->GetMaterial()->texture) {
            resultIntensity*=closestIntersection.object->GetMaterial()->texture->SampleSpherical(closestIntersection.object->MapToLocal(closestIntersection.point));
        }
    }
    return resultIntensity;
}

LightIntensity RayTracer::TraceRayStream(const Ray&ray, Scene*scene,
                                   const Vector3 cameraPosition,
                                   int reflections, float exposure,
                                   StreamPhotonMap* globalMap,
                                   StreamPhotonMap* causticMap) {
    LightIntensity resultIntensity;

    int closest=-1;
    float closestDist=FLT_MAX;
    IntersectionResult closestIntersection;

    IntersectionResult result;
    for(int j=0;j<scene->geometry.count();j++) {
        result = scene->geometry.at(j)->Intersects(ray);
        if(result.type!=MISS) {
            if(closestDist>result.distance) {
                closestDist = result.distance;
                closest = j;
                closestIntersection = result;
            }
        }
    }
    if(closest!=-1) {
        if(closestIntersection.object->GetMaterial()->type==REFLECTIVE && reflections>0) {
            Vector3 reflected = ray.direction.Reflect(closestIntersection.intersectionPointNormal);
            reflected.Normalize();
            Ray newRay(closestIntersection.point+reflected*BIAS, reflected);

            resultIntensity += TraceRayStream(newRay, scene, cameraPosition, reflections-1, exposure, globalMap,causticMap);
        }
        else if(closestIntersection.object->GetMaterial()->type==REFRACTIVE && reflections>0) {
            float reflectionCoef = max(0.0, min(1.0, 0.05+0.11*(pow(1+ray.direction.DotProduct(closestIntersection.intersectionPointNormal), 1))));
            LightIntensity reflectedIntensity;
            LightIntensity refractedIntensity;

            RefractiveMaterial* mat = (RefractiveMaterial*)closestIntersection.object->GetMaterial();

            Vector3 refracted;
            if(closestIntersection.type==HIT)
                refracted = ray.direction.Refract(closestIntersection.intersectionPointNormal,
                                                  mat->etaRate);
            else
                refracted = ray.direction.Refract(-closestIntersection.intersectionPointNormal,
                                                  1.0f/mat->etaRate);
            refracted.Normalize();

            Ray newRay(closestIntersection.point+refracted*BIAS, refracted);

            refractedIntensity += TraceRayStream(newRay, scene, cameraPosition, reflections-1, exposure, globalMap,causticMap);

            Vector3 reflected = ray.direction.Reflect(closestIntersection.intersectionPointNormal);
            reflected.Normalize();
            newRay = Ray(closestIntersection.point+reflected*BIAS, reflected);

            reflectedIntensity += TraceRayStream(newRay, scene, cameraPosition, reflections-1, exposure, globalMap,causticMap);


            resultIntensity = reflectionCoef*reflectedIntensity + (1-reflectionCoef)*refractedIntensity;
        }
        else {
            LightIntensity fromLights;
            for(int j=0;j<scene->lights.count();j++) {
                fromLights += scene->lights.at(j)->GetLightIntensity(cameraPosition, &closestIntersection, scene->geometry);
            }
            resultIntensity += fromLights;

            if(globalMap) {
                QList<Stream*> closest = globalMap->GetClosestPhotons(closestIntersection.point, globalRadius, globalNumPhotons);

                if(closest.count()!=0) {
                    float r = (closest[0]->position-closestIntersection.point).GetLength();

                    LightIntensity E;
                    for(int z=0;z<closest.count();z++) {
                        for(int j=0;j<closest[z]->associatedPhoton.count();j++) {
                            float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->associatedPhoton[j].direction);
                            if(dot>0) {
                                E += exposure*closest[z]->associatedPhoton[j].energy*dot;

                            }
                        }

                        float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->leadingPhoton.direction);
                        if(dot>0) {
                            E += exposure*closest[z]->leadingPhoton.energy*dot;
                        }
                    }

                    resultIntensity += (E/(M_PI*r*r));
                }
            }

            if(causticMap) {
                QList<Stream*> closest = causticMap->GetClosestPhotons(closestIntersection.point, causticRadius, causticNumPhotons);

                if(closest.count()!=0) {
                    float r = (closest[0]->position-closestIntersection.point).GetLength();

                    LightIntensity E;
                    for(int z=0;z<closest.count();z++) {
                        for(int j=0;j<closest[z]->associatedPhoton.count();j++) {
                            float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->associatedPhoton[j].direction);
                            if(dot>0) {
                                E += exposure*closest[z]->associatedPhoton[j].energy*dot;

                            }
                        }

                        float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->leadingPhoton.direction);
                        if(dot>0) {
                            E += exposure*closest[z]->leadingPhoton.energy*dot;
                        }
                    }

                    resultIntensity += (E/(M_PI*r*r));
                }
            }

        }
        if(closestIntersection.object->GetMaterial()->texture) {
            resultIntensity*=closestIntersection.object->GetMaterial()->texture->SampleSpherical(closestIntersection.object->MapToLocal(closestIntersection.point));
        }
    }
    return resultIntensity;
}




