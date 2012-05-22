#ifndef RAY_H
#define RAY_H

#include "Vector3.h"
#include <QDebug>
class Geometry;

class Ray
{
public:
    Vector3 origin;
    Vector3 direction;

    Ray(void);
    Ray(const Vector3& orig, const Vector3& dir);    
};

enum IntersectionType {
    MISS = 0,
    HIT,
    INSIDE_PRIMITIVE
};

struct IntersectionResult {
    IntersectionResult() {
        type=MISS;
        object=0;
    }

    IntersectionResult(IntersectionType t,
                       Vector3 p,
                       Vector3 n,
                       float dist) : type(t), point(p),
                                     intersectionPointNormal(n), distance(dist) {}
    IntersectionType type;
    Vector3 point;
    Vector3 intersectionPointNormal;
    float distance;
    const Geometry* object;
};


QDebug operator<<(QDebug dbg, const Ray &r);
QDebug operator<<(QDebug dbg, const IntersectionResult &ir);

#endif // RAY_H
