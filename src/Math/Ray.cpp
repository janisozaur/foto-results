#include <Math/Ray.h>

#include <Math/Plane.h>

Ray::Ray(void)
{
    origin = Vector3(0,0,0);
    direction = Vector3(0,0,1);
}

Ray::Ray(const Vector3 &orig, const Vector3 &dir) : origin(orig), direction(dir) {
    direction.Normalize();
}

QDebug operator<<(QDebug dbg, const Ray &r) {
    dbg.nospace() << "( origin" << r.origin <<", dir" << r.direction <<")";
    return dbg.space();
}


QDebug operator<<(QDebug dbg, const IntersectionResult &ir) {
    if(ir.type==MISS) {
        dbg.nospace() <<"no intersection";
    }
    else if(ir.type==HIT) {
        dbg.nospace() <<"intersection at point "<<ir.point<<", distance"<<ir.distance<<", normal at that point"<<ir.intersectionPointNormal;
    }
    else {
        dbg.nospace() <<"intersection at point inside primitive "<<ir.point<<", distance "<<ir.distance<<", normal at that point"<<ir.intersectionPointNormal;
    }

    return dbg.space();
}
