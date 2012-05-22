#include "PointLight.h"
#include <algorithm>
#include <cfloat>
#include <Scene.h>

using namespace std;

PointLight::PointLight() : AmbientLight()
{
    position.Zero();
    attenuation.One();
    type = POINT;
    projectionMap=0;
}

PointLight::PointLight(Vector3 position, Color color, Vector3 attenuation) : AmbientLight() {
    this->position = position;
    this->color = color;
    this->attenuation = attenuation;
    type = POINT;
    projectionMap=0;
}

PointLight::~PointLight() {
    if(projectionMap) {
        delete projectionMap;
        projectionMap=0;
    }
}

bool PointLight::IsInShadow(IntersectionResult *ir, QList<Geometry *> &geometry) {
    //chec if ray from light to intersection point intersects some geometry
    Ray lightToPoint(position, ir->point-position);
    float dist = (position-ir->point).GetLength() - 0.001;

    for(int i=0;i<geometry.count();i++) {
        IntersectionResult intersect = geometry.at(i)->Intersects(lightToPoint);
        if(intersect.type==HIT && intersect.distance<dist && intersect.object!=ir->object)
            return true;
    }

    return false;
}

Vector3 PointLight::GetPosition() const {
    return position;
}

LightIntensity PointLight::GetLightIntensity(Vector3 cameraPosition, IntersectionResult *ir,
                                             QList<Geometry *> &geometry) {
    LightIntensity result(0,0,0);

    if(IsInShadow(ir, geometry))
        return result;

    Vector3 normal = ir->intersectionPointNormal;
    Vector3 light(position - ir->point);

    float lightDistance = light.GetLength();
    normal.Normalize();
    light.Normalize();

    Material* mat = ir->object->GetMaterial();
    //if geometry has diffuse material calculate phong lighting
    if(mat->type==DIFFUSE) {
        DiffuseMaterial* diffMat = (DiffuseMaterial*)mat;
        float diffuseFactor = normal.DotProduct(light);

        if(diffuseFactor > 0.0f) {
            float specPower = diffMat->specularCoeff;
            Vector3 eye = cameraPosition - ir->point;
            eye.Normalize();
            Vector3 reflected = (-light).Reflect(normal);
            float specFactor = pow(max(reflected.DotProduct(eye), 0.0f), specPower);

            result += diffMat->diffuse*color*diffuseFactor;
            result += diffMat->specular*color*specFactor;
        }
        result /= (attenuation.x+attenuation.y*lightDistance+attenuation.z*lightDistance*lightDistance);
    }

    return result;
}


//generate photon from unit sphere
Ray PointLight::GetPhoton(bool useProjectionMap) const {
    float x,y,z;
    bool ok=false;
    do {
        x = 2.0f*((float)qrand())/RAND_MAX-1.0f;
        y = 2.0f*((float)qrand())/RAND_MAX-1.0f;
        z = 2.0f*((float)qrand())/RAND_MAX-1.0f;

        if(x*x+y*y+z*z<=1) {
            if(useProjectionMap && projectionMap) {
                Vector3 dir(x,y,z);
                dir.Normalize();

                /*if(hemisphere) {
                    if(dir.DotProduct(Vector3(0,-1,0)))
                        dir*=-1;
                }
*/
                //if we use projection map
                //generated photon must be directed into reflective/refractive geometry
                if(projectionMap->SampleSpherical(dir)==Color(1,1,1))
                    ok = true;
            }
            else {
                ok = true;
            }
        }
    }
    while(!ok);


    return Ray(position, Vector3(x,y,z));
}


void PointLight::CreateProjectionMap(const Scene* scene) {
    if(projectionMap)
        delete projectionMap;


    projectionMap = new Texture(512,512);

    //for each pixel in projection map
    for(int y=0;y<projectionMap->GetHeight();y++) {
        for(int x=0;x<projectionMap->GetWidth();x++) {
            float u = (float)x/projectionMap->GetWidth();
            float v = (float)y/projectionMap->GetHeight();

            u = 1.0f-u;
            v = 1.0f-v;

            //calculate spherical coordinates
            float phi = u*2*M_PI;
            float theta = (1.0f-v)*M_PI;


            Vector3 direction(sin(theta)*sin(phi), cos(theta), sin(theta)*cos(phi));

            //chec if ray for given pixel intersects reflective/refractive geometry
            Ray ray(position, direction);

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

            if(closest!=-1 && (closestIntersection.object->GetMaterial()->type == REFRACTIVE || closestIntersection.object->GetMaterial()->type == REFLECTIVE))
                projectionMap->SetPixel(x,y,Color(1,1,1));
            else
                projectionMap->SetPixel(x,y,Color(0,0,0));
        }
    }

    projectionMap->SaveToFile("projectionMap.png");
}


float PointLight::GetProjectionMapRatio() const {
    if(projectionMap)
        return projectionMap->GetWhiteToBlackPixelRatio();
    return 0;
}
