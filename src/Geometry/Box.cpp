#include "Box.h"
#include <cfloat>

Box::Box(Vector3 pos, Vector3 size) {
    Vector3 vertices[8];
    //calcilate tiangles for each wall
    vertices[0] = pos + Vector3(size.x/2, size.y/2, size.z/2);
    vertices[1] = pos + Vector3(size.x/2, -size.y/2, size.z/2);
    vertices[2] = pos + Vector3(size.x/2, -size.y/2, -size.z/2);
    vertices[3] = pos + Vector3(size.x/2, size.y/2, -size.z/2);

    vertices[4] = pos + Vector3(-size.x/2, size.y/2, size.z/2);
    vertices[5] = pos + Vector3(-size.x/2, -size.y/2, size.z/2);
    vertices[6] = pos + Vector3(-size.x/2, -size.y/2, -size.z/2);
    vertices[7] = pos + Vector3(-size.x/2, size.y/2, -size.z/2);

    Triangle* t;
    t = new Triangle(vertices[0], vertices[1], vertices[2]);

    walls.append(t);

    t = new Triangle(vertices[2], vertices[3], vertices[0]);

    walls.append(t);


    t = new Triangle(vertices[6], vertices[5], vertices[4]);

    walls.append(t);

    t = new Triangle(vertices[4], vertices[7], vertices[6]);

    walls.append(t);


    //floor
    t = new Triangle(vertices[6], vertices[2], vertices[1]);

    walls.append(t);

    t = new Triangle(vertices[1], vertices[5], vertices[6]);

    walls.append(t);

    //roof
    t = new Triangle(vertices[7], vertices[4], vertices[0]);

    walls.append(t);

    t = new Triangle(vertices[0], vertices[3], vertices[7]);

    walls.append(t);

    //back
    t = new Triangle(vertices[0], vertices[4], vertices[5]);

    walls.append(t);

    t = new Triangle(vertices[5], vertices[1], vertices[0]);

    walls.append(t);


    //front
    t = new Triangle(vertices[6], vertices[7], vertices[3]);
    walls.append(t);

    t = new Triangle(vertices[3], vertices[2], vertices[6]);
    walls.append(t);
}

IntersectionResult Box::Intersects(const Ray &ray, float range) const {
    IntersectionResult closest;
    float closestDist = FLT_MAX;

    for(int i=0;i<walls.count();i++) {
        IntersectionResult ir = walls.at(i)->Intersects(ray, range);
        if(ir.type!=MISS && ir.distance < closestDist) {
            closest = ir;
            closestDist = ir.distance;
        }
    }
    return closest;
}

void Box::SetMaterial(Material *m) {
    for(int i=0;i<walls.count();i++) {
        walls[i]->SetMaterial(m);
    }

    this->material = m;
}

Box::~Box() {
    for(int i=0;i<walls.count();i++) {
        delete walls.at(i);
    }

    walls.clear();
}
