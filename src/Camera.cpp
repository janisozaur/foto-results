#include "Camera.h"
#include <algorithm>
#include <cfloat>
#include "PhotonMap.h"
#include "StreamPhotonMap.h"

#define BIAS 0.001f
using namespace std;
Camera::Camera()
{
    position.Zero();
    target.Zero();
    target.z = 1;
    nearPlane = 0.001;
    farPlane = 1000;
    fov = 54;
    img = 0;
    renderFileName = "render";
}

Camera::Camera(Vector3 positon, Vector3 target, float nearPlane, float farPlane, float fov) {
    this->position = positon;
    this->target = target;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->fov = fov;
    img=0;
    renderFileName = "render";
}


/**
  Recalculates camera projection and view matrices.
  */
void Camera::Recalculate() {
    if(img) {
        projectionMatrix.SetPerspective(fov, ((float)img->GetWidth())/img->GetHeight(), nearPlane, farPlane);

        distance = 1.0f/tanf(fov/2);
        viewMatrix.LoadIdentity();

        Vector3 direction = target - position;
        direction.Normalize();

        //up vector is always up
        Vector3 up = Vector3(0,1,0);

        Vector3 left = up.CrossProduct(direction);
        left.Normalize();

        viewMatrix.SetRow(0, Vector4(left, 0.0f));
        viewMatrix.SetRow(1, Vector4(up, 0.0f));
        viewMatrix.SetRow(2, Vector4(direction, 0.0f));
        viewMatrix.SetRow(3, Vector4(0, 0, 0, 1));

        Matrix4x4 trans;
        trans.SetTranslation(-position);
        viewMatrix = viewMatrix*trans;

        vpMatrix = viewMatrix;
        invVPMatrix = vpMatrix.GetInverse();
    }
}



timespec GetCurrenTime() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t;
}

float GetDiffInMsec(const timespec &start, const timespec &end) {
    timespec res;
    res.tv_sec  = end.tv_sec  - start.tv_sec;
    res.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (res.tv_nsec < 0) {
        res.tv_nsec += 1e9;
        res.tv_sec  -= 1;
    }

    float msec = res.tv_sec*1000;
    msec += (float)res.tv_nsec/1e6;

    return msec;
}

void Camera::RenderScene(Scene* scene, unsigned int ns) {
    timespec startTime = GetCurrenTime();
    Recalculate();

    float pixelW = 1.0f/img->GetWidth();
    float pixelH = 1.0f/img->GetHeight();

    int numSamples=ns;
    if(img) {
        img->Clear(LightIntensity(0,0,0));


        #pragma omp parallel for schedule(dynamic, 50)
        for(unsigned int i=0;i<img->GetWidth()*img->GetHeight();i++) {
            float x = i % img->GetWidth();
            float y = i / img->GetWidth();

            LightIntensity currentPixel;
            for(int sY=-numSamples/2;sY<=numSamples/2;sY++) {
                for(int sX=-numSamples/2;sX<=numSamples/2;sX++) {

                    //cast ray into image
                    float px = 2.0f*((x+pixelW/numSamples*sX)/img->GetWidth()) - 1.0;
                    float py = 2.0f*((y+pixelH/numSamples*sY)/img->GetHeight()) - 1.0;
                    py*=-1;
                    px /= projectionMatrix.entries[0];
                    py /= projectionMatrix.entries[5];

                    Vector4 origin(0,0,0,1);
                    Vector4 direction(px,py, 1, 0);

                    origin = invVPMatrix*Vector4(origin);
                    direction = invVPMatrix*Vector4(direction);

                    Ray ray(Vector3(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z));

                    //and trace it
                    currentPixel+=rayTracer.TraceRay(ray, scene, position, 6);
                }
            }
            img->SetPixel(x,y,currentPixel/(numSamples*numSamples));
        }
        timespec endTime = GetCurrenTime();

        qDebug()<<"Rendering time"<<GetDiffInMsec(startTime, endTime)<<"ms";
        img->SaveToFile(renderFileName);
    }
}

void Camera::RenderSceneStream(Scene* scene, unsigned int ns, unsigned int numGlobalMapPhotons,
                               unsigned int numCausticMapPhotons, int numAssociatedPhotons, float radius) {
    timespec startTime;

    Recalculate();

    float pixelW = 1.0f/img->GetWidth();
    float pixelH = 1.0f/img->GetHeight();

    int numSamples=ns;
    if(img) {
        img->Clear(LightIntensity(0,0,0));

        StreamPhotonMap photonMap;
        photonMap.SetNumAssociatedPhotons(numAssociatedPhotons);
        photonMap.SetRadius(radius);

        photonMap.GeneratePhotonMap(scene, numGlobalMapPhotons, 6);

        StreamPhotonMap causticPhotonMap;
        causticPhotonMap.SetNumAssociatedPhotons(numAssociatedPhotons);
        causticPhotonMap.SetRadius(radius);
        causticPhotonMap.GeneratePhotonMap(scene, numCausticMapPhotons, 2, true);
        startTime = GetCurrenTime();
        #pragma omp parallel for schedule(dynamic, 50)
        for(unsigned int i=0;i<img->GetWidth()*img->GetHeight();i++) {
            float x = i % img->GetWidth();
            float y = i / img->GetWidth();

            LightIntensity currentPixel;
            for(int sY=-numSamples/2;sY<=numSamples/2;sY++) {
                for(int sX=-numSamples/2;sX<=numSamples/2;sX++) {
                    float px = 2.0f*((x+pixelW/numSamples*sX)/img->GetWidth()) - 1.0;
                    float py = 2.0f*((y+pixelH/numSamples*sY)/img->GetHeight()) - 1.0;
                    py*=-1;
                    px /= projectionMatrix.entries[0];
                    py /= projectionMatrix.entries[5];

                    Vector4 origin(0,0,0,1);
                    Vector4 direction(px,py, 1, 0);

                    origin = invVPMatrix*Vector4(origin);
                    direction = invVPMatrix*Vector4(direction);

                    Ray ray(Vector3(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z));

                    currentPixel+=rayTracer.TraceRayStream(ray, scene, position, 6, 750, &photonMap, &causticPhotonMap);
                }
            }
            img->SetPixel(x,y,currentPixel/(numSamples*numSamples));
        }
        timespec endTime = GetCurrenTime();

        qDebug()<<"Rendering time"<<(int)GetDiffInMsec(startTime, endTime)/1000<<"s";
        img->SaveToFile(renderFileName);
    }
}

void Camera::RenderScene(Scene* scene, unsigned int ns, unsigned int numGlobalMapPhotons,
                         unsigned int numCausticMapPhotons) {


    timespec startTime;
    Recalculate();

    float pixelW = 1.0f/img->GetWidth();
    float pixelH = 1.0f/img->GetHeight();

    int numSamples=ns;
    if(img) {
        img->Clear(LightIntensity(0,0,0));

        PhotonMap photonMap;

        photonMap.GeneratePhotonMap(scene, numGlobalMapPhotons, 6);

        PhotonMap causticPhotonMap;
        causticPhotonMap.GeneratePhotonMap(scene, numCausticMapPhotons, 2, true);
        startTime = GetCurrenTime();
        #pragma omp parallel for schedule(dynamic, 50)
        for(unsigned int i=0;i<img->GetWidth()*img->GetHeight();i++) {
            float x = i % img->GetWidth();
            float y = i / img->GetWidth();

            LightIntensity currentPixel;
            for(int sY=-numSamples/2;sY<=numSamples/2;sY++) {
                for(int sX=-numSamples/2;sX<=numSamples/2;sX++) {
                    float px = 2.0f*((x+pixelW/numSamples*sX)/img->GetWidth()) - 1.0;
                    float py = 2.0f*((y+pixelH/numSamples*sY)/img->GetHeight()) - 1.0;
                    py*=-1;
                    px /= projectionMatrix.entries[0];
                    py /= projectionMatrix.entries[5];

                    Vector4 origin(0,0,0,1);
                    Vector4 direction(px,py, 1, 0);

                    origin = invVPMatrix*Vector4(origin);
                    direction = invVPMatrix*Vector4(direction);

                    Ray ray(Vector3(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z));

                    currentPixel+=rayTracer.TraceRay(ray, scene, position, 6, 750, &photonMap,&causticPhotonMap);
                }
            }
            img->SetPixel(x,y,currentPixel/(numSamples*numSamples));
        }
        timespec endTime = GetCurrenTime();

        qDebug()<<"Rendering time"<<(int)GetDiffInMsec(startTime, endTime)/1000<<"s";
        img->SaveToFile(renderFileName);
    }
}

void Camera::VisualizePhotonMap(Scene *scene, int numPhotons, int maxReflections) {
     Recalculate();
    if(img) {
        img->Clear(LightIntensity(0,0,0));

        PhotonMap photonMap;

        photonMap.GeneratePhotonMap(scene, numPhotons, maxReflections, true);

        #pragma omp parallel for
        for(unsigned int i=0;i<img->GetWidth()*img->GetHeight();i++) {
            float x = i % img->GetWidth();
            float y = i / img->GetWidth();
            float X,Y;
            X=x;
            Y=y;

            x= 2.0f*(x/img->GetWidth())-1.0f;
            y= 2.0f*(y/img->GetHeight())-1.0f;
            y*=-1;
            x /= projectionMatrix.entries[0];
            y /= projectionMatrix.entries[5];

            Vector4 origin(0,0,0,1);
            Vector4 direction(x,y, 1, 0);

            origin = invVPMatrix*Vector4(origin);
            direction = invVPMatrix*Vector4(direction);

            Ray ray(Vector3(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z));

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
                if(closestIntersection.object->GetMaterial()->type==REFLECTIVE) {
                    resultIntensity = LightIntensity(0,0,0);
                }
                else if(closestIntersection.object->GetMaterial()->type==REFRACTIVE) {

                    resultIntensity = LightIntensity(0,0,0);
                }
                else {
                    QList<Photon*> closest = photonMap.GetClosestPhotons(closestIntersection.point, 0.5, 2);

                    if(closest.count()!=0) {
                        float r = (closest[0]->position-closestIntersection.point).GetLength();

                        LightIntensity E;
                        for(int z=0;z<closest.count();z++) {
                            float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->direction);
                            if(dot>0) {
                                E += 2000*closest[z]->energy*dot;
                            }
                        }


                        resultIntensity += (E/(M_PI*r*r));
                    }
                }
            }
            img->SetPixel(X,Y,resultIntensity);
        }
        img->SaveToFile(renderFileName);
    }
}


void Camera::VisualizeStreamPhotonMap(Scene *scene, int numPhotons, int maxReflections, int numAssociatedPhotons, float radius) {
    Recalculate();
    if(img) {
        img->Clear(LightIntensity(0,0,0));

        StreamPhotonMap photonMap;
        photonMap.SetNumAssociatedPhotons(numAssociatedPhotons);
        photonMap.SetRadius(radius);

        photonMap.GeneratePhotonMap(scene, numPhotons, maxReflections, false);

        #pragma omp parallel for
        for(unsigned int i=0;i<img->GetWidth()*img->GetHeight();i++) {
            float x = i % img->GetWidth();
            float y = i / img->GetWidth();
            float X,Y;
            X=x;
            Y=y;

            x= 2.0f*(x/img->GetWidth())-1.0f;
            y= 2.0f*(y/img->GetHeight())-1.0f;
            y*=-1;
            x /= projectionMatrix.entries[0];
            y /= projectionMatrix.entries[5];

            Vector4 origin(0,0,0,1);
            Vector4 direction(x,y, 1, 0);

            origin = invVPMatrix*Vector4(origin);
            direction = invVPMatrix*Vector4(direction);

            Ray ray(Vector3(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z));

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
                if(closestIntersection.object->GetMaterial()->type==REFLECTIVE) {
                    resultIntensity = LightIntensity(0,0,0);
                }
                else if(closestIntersection.object->GetMaterial()->type==REFRACTIVE) {

                    resultIntensity = LightIntensity(0,0,0);
                }
                else {
                    QList<Stream*> closest = photonMap.GetClosestPhotons(closestIntersection.point, 0.1, 2);

                    if(closest.count()!=0) {
                        float r = (closest[0]->position-closestIntersection.point).GetLength();

                        LightIntensity E;
                        for(int z=0;z<closest.count();z++) {
                            //qDebug()<<closest[z]->associatedPhoton.count();
                            for(int j=0;j<closest[z]->associatedPhoton.count();j++) {
                                float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->associatedPhoton[j].direction);
                                if(dot>0) {
                                    E += 2000*closest[z]->associatedPhoton[j].energy*dot;
                                }
                            }

                            float dot = closestIntersection.intersectionPointNormal.DotProduct(closest[z]->leadingPhoton.direction);
                            if(dot>0) {
                                E += 2000*closest[z]->leadingPhoton.energy*dot;
                            }
                        }

                        resultIntensity += (E/(M_PI*r*r));
                    }
                }
            }
            img->SetPixel(X,Y,resultIntensity);
        }
        img->SaveToFile(renderFileName);
    }
}
