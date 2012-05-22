#include "AreaLight.h"

AreaLight::AreaLight(Vector3 center, Vector2 size, Color color, Vector3 attenuation, int density) {
    float stepX = size.x/(density);
    float stepY = size.y/(density);
    Vector3 corner = center - Vector3(size.x, 0, size.y);

    for(int y=-density/2;y<=density/2;y++) {
        for(int x=-density/2;x<=density/2;x++) {
            PointLight* p = new PointLight(center+Vector3(x*stepX, 0, y*stepY), color/(density*density), attenuation);
            lights.push_back(p);
        }
    }
    inShadows.resize(lights.count());

    this->center = center;
    this->size = size;

    type = AREA;
}


AreaLight::~AreaLight() {
    for(int i=0;i<lights.count();i++)
        delete lights[i];
    lights.clear();
    inShadows.clear();
}

bool AreaLight::IsInShadow(IntersectionResult *ir, QList<Geometry *> &geometry) {
    return false;
}

LightIntensity AreaLight::GetLightIntensity(Vector3 cameraPosition,
                                            IntersectionResult *ir,
                                            QList<Geometry *> &geometry) {
    LightIntensity result;
    for(int i=0;i<lights.count();i++) {
        //if(!inShadows.at(i))
        result+=lights.at(i)->GetLightIntensity(cameraPosition, ir, geometry);
    }
    return result;///lights.count();
}

Ray AreaLight::GetPhoton(bool useProjectionMap) const {

    int i = qrand()%lights.count();

    Ray r = lights.at(i)->GetPhoton(useProjectionMap);

    if(r.direction.DotProduct(Vector3(0,-1,0))<0) {
        r = Ray(r.origin, -r.direction);
    }

    return r;
}

void AreaLight::CreateProjectionMap(const Scene* scene) {
    for(int i=0;i<lights.count();i++) {
        qDebug()<<i;
        lights.at(i)->CreateProjectionMap(scene);
    }
}

float AreaLight::GetProjectionMapRatio() const {
    float avg=0;
    for(int i=0;i<lights.count();i++)
        avg += lights.at(i)->GetProjectionMapRatio();

    return avg/lights.count();
}
