#include "CornellBox.h"
#include <cfloat>

CornellBox::CornellBox(Vector3 pos, Vector3 size) {
    redWallMaterial = new DiffuseMaterial(Color(0,0,0), Color(1,0,0), Color(0.0,0.0,0.0), 0, 0.5);
    greenWallMaterial = new DiffuseMaterial(Color(0,0,0), Color(0,1,0), Color(0.0,0.0,0.0), 0, 0.5);
    whiteWallMaterial = new DiffuseMaterial(Color(0,0,0), Color(1,1,1), Color(0.0,0.0,0.0), 0, 0.5);

    Vector3 vertices[8];
    vertices[0] = pos + Vector3(size.x/2, size.y/2, size.z/2);
    vertices[1] = pos + Vector3(size.x/2, -size.y/2, size.z/2);
    vertices[2] = pos + Vector3(size.x/2, -size.y/2, -size.z/2);
    vertices[3] = pos + Vector3(size.x/2, size.y/2, -size.z/2);

    vertices[4] = pos + Vector3(-size.x/2, size.y/2, size.z/2);
    vertices[5] = pos + Vector3(-size.x/2, -size.y/2, size.z/2);
    vertices[6] = pos + Vector3(-size.x/2, -size.y/2, -size.z/2);
    vertices[7] = pos + Vector3(-size.x/2, size.y/2, -size.z/2);

    Triangle* t;
    t = new Triangle(vertices[2], vertices[1], vertices[0]);
    t->SetMaterial(greenWallMaterial);
    walls.append(t);

    t = new Triangle(vertices[0], vertices[3], vertices[2]);
    t->SetMaterial(greenWallMaterial);
    walls.append(t);


    t = new Triangle(vertices[4], vertices[5], vertices[6]);
    t->SetMaterial(redWallMaterial);
    walls.append(t);

    t = new Triangle(vertices[6], vertices[7], vertices[4]);
    t->SetMaterial(redWallMaterial);
    walls.append(t);


    //floor
    t = new Triangle(vertices[1], vertices[2], vertices[6]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

    t = new Triangle(vertices[6], vertices[5], vertices[1]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

    //roof
    t = new Triangle(vertices[0], vertices[4], vertices[7]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

    t = new Triangle(vertices[7], vertices[3], vertices[0]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

    //back
    t = new Triangle(vertices[5], vertices[4], vertices[0]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

    t = new Triangle(vertices[0], vertices[1], vertices[5]);
    t->SetMaterial(whiteWallMaterial);
    walls.append(t);

}

IntersectionResult CornellBox::Intersects(const Ray &ray, float range) const {
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

CornellBox::~CornellBox() {
    delete redWallMaterial;
    delete greenWallMaterial;
    delete whiteWallMaterial;

    for(int i=0;i<walls.count();i++) {
        delete walls.at(i);
    }

    walls.clear();
}
