#include "Plane.h"
#include "Vector3.h"
#include <Math/Ray.h>

void Plane::SetFromPoints(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2) {
    normal=(p1-p0).CrossProduct(p2-p0);

    normal.Normalize();

    CalculateDistance(p0);
}

void Plane::Normalize() {
    float normalLength=normal.GetLength();
    normal/=normalLength;
    distance/=normalLength;
}

bool Plane::Intersect3(const Plane &p2, const Plane &p3, Vector3 &result) {
//find point of intersection of 3 Planes
    float denominator=normal.DotProduct((p2.normal).CrossProduct(p3.normal));
											//scalar triple product of normals
    if(denominator==0.0f)									//if zero
        return false;										//no intersection

    Vector3 temp1, temp2, temp3;
    temp1=(p2.normal.CrossProduct(p3.normal))*distance;
    temp2=(p3.normal.CrossProduct(normal))*p2.distance;
    temp3=(normal.CrossProduct(p2.normal))*p3.distance;

    result=(temp1+temp2+temp3)/(-denominator);

    return true;
}

float Plane::GetDistanceFromPoint(const Vector3 &point) const {
    return point.x*normal.x + point.y*normal.y + point.z*normal.z + distance;
}

int Plane::ClassifyPoint(const Vector3 &point) const {
    float distanceFromPoint=point.x*normal.x + point.y*normal.y + point.z*normal.z + distance;

    if(distanceFromPoint>EPSILON)	//==0.0f is too exact, give a bit of room
        return POINT_IN_FRONT_OF_PLANE;
	
    if(distanceFromPoint<-EPSILON)
        return POINT_BEHIND_PLANE;

    return POINT_ON_PLANE;	//otherwise
}

Plane Plane::Lerp(const Plane &p2, float factor) {
    Plane result;
    result.normal=normal*(1.0f-factor) + p2.normal*factor;
    result.normal.Normalize();

    result.distance=distance*(1.0f-factor) + p2.distance*factor;

    return result;
}

bool Plane::operator ==(const Plane &plane) const {
    if(normal==plane.normal && distance==plane.distance)
        return true;

    return false;
}

IntersectionResult Plane::Intersects(const Ray& ray, float range) const {
    IntersectionResult result;
    result.type = MISS;
    float ndotD = normal.DotProduct(ray.direction);
    if(ndotD==0) {
        return result;
    }

    float t;
    t = (-distance - normal.DotProduct(ray.origin))/ndotD;

    if(t>0) {
        if(range==0.0f || t<range) {
            if(normal.DotProduct(ray.direction)<0)
                result.type=HIT;
            else
                result.type = INSIDE_PRIMITIVE;
            result.point = ray.origin + t*ray.direction;
            result.distance = t;
            result.intersectionPointNormal = normal;
        }
    }

    return result;
}
