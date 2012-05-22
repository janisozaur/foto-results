#include "Triangle.h"
#include "Math/Plane.h"
//if this flag is defined, we use barycentric coordinates to interpolate normal values at given triangle point.
#define VERTEX_NORMALS


Triangle::Triangle() : Geometry() {
    vertices[0].Zero();
    vertices[1].Zero();
    vertices[2].Zero();

    normals[0].One();
    normals[1].One();
    normals[2].One();
}

Triangle::Triangle(const Vector3& vertex1,
                   const Vector3& vertex2,
                   const Vector3& vertex3) : Geometry() {
    vertices[0] = vertex1;
    vertices[1] = vertex2;
    vertices[2] = vertex3;

    Vector3 normal = (vertex2 - vertex1).CrossProduct(vertex3 - vertex1);
    normal.Normalize();

    normals[0] = normal;
    normals[1] = normal;
    normals[2] = normal;
}

Triangle::Triangle(const Vector3 &vertex1,
                   const Vector3 &vertex2,
                   const Vector3 &vertex3,
                   const Vector3 &normal1,
                   const Vector3 &normal2,
                   const Vector3 &normal3) {
    vertices[0] = vertex1;
    vertices[1] = vertex2;
    vertices[2] = vertex3;

    normals[0] = normal1;
    normals[1] = normal2;
    normals[2] = normal3;
}

IntersectionResult Triangle::Intersects(const Ray& ray, float range) const {
    //Create plane from three vertices of triangle
    Plane p;
    p.SetFromPoints(vertices[0], vertices[1], vertices[2]);

    //Check if ray intersects with this plane first
    IntersectionResult planeIntersect = p.Intersects(ray, range);
    if(planeIntersect.type==MISS)
        return planeIntersect;

    //check if intersection is in triangle
    Vector3 fa = vertices[0] - planeIntersect.point;
    Vector3 fb = vertices[1] - planeIntersect.point;
    Vector3 fc = vertices[2] - planeIntersect.point;

    Vector3 x;
    x = fa.CrossProduct(fb);
    if(x.DotProduct(p.normal)<-EPSILON)
        return IntersectionResult();

    x = fb.CrossProduct(fc);
    if(x.DotProduct(p.normal)<-EPSILON)
        return IntersectionResult();

    x = fc.CrossProduct(fa);
    if(x.DotProduct(p.normal)<-EPSILON)
        return IntersectionResult();

#ifdef VERTEX_NORMALS
    //calculate barycentric coordinantes
    float triArea = (vertices[1] - vertices[0]).CrossProduct(vertices[2] - vertices[0]).GetLength() * 0.5f;
    float u =       ((vertices[1] - planeIntersect.point ).CrossProduct(vertices[2] - planeIntersect.point).GetLength() * 0.5 ) / triArea;
    float v =       ((vertices[0] - planeIntersect.point ).CrossProduct(vertices[2] - planeIntersect.point).GetLength() * 0.5 ) / triArea;
    float w =       ((vertices[0] - planeIntersect.point ).CrossProduct(vertices[1] - planeIntersect.point).GetLength() * 0.5 ) / triArea;

    //interpolate normal at intersection point
    planeIntersect.intersectionPointNormal = u*normals[0]+v*normals[1]+w*normals[2];
    planeIntersect.intersectionPointNormal.Normalize();

#endif
    planeIntersect.object = this;
    return planeIntersect;
}
