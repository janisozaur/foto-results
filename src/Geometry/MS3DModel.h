#ifndef __MS3DMODEL_H__
#define __MS3DMODEL_H__
#include <stdlib.h>
#include <vector>
#include <Math/Vector3.h>
#include "Geometry.h"

using namespace std;

class MS3DVertex
{
public:
	float position[3];
	char boneIndex;
};


class MS3DTriangle
{
public:
	unsigned short vertexIndices[3];
	float vertexNormal[3][3];
	float normal[3];
	float s[3];
	float t[3];
	char groupIndex;
};

class MS3DMesh
{
public:
	unsigned short numTriangles;
	unsigned short *trianglesIndices;
	char materialIndex;
};

class MS3DMaterial
{
public:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shinniness;
	float transparency;
	char texture[128];
	char alphamap[128];
        //GLuint textureID;
};


class MS3DKeyFrameRotation
{
public:
	float time;
	float rotation[3];
};

class MS3DKetFramePosition
{
public:
	float time;
	float position[3];
};


/**
  Loads geometry from milkshape 3d model
  */
class MS3DModel : public Geometry
{
	unsigned short numMeshes;
	unsigned short numTriangles;
	unsigned short numMaterials;
	unsigned short numVertices;
	unsigned short numJoints;

    MS3DVertex * vertices;
    MS3DTriangle * triangles;
    MS3DMaterial *materials;
    MS3DMesh * meshes;
    Vector3 position;
    float scale;

    Vector3 bboxMin;
    Vector3 bboxMax;

public:
    /**
      Loads model from file
      \param filename path to file
      */
    void Load(const char*filename);
    /**
      Sets position and scale of geometry
      */
    void SetPositionAndScale(const Vector3& position, float scale);

    MS3DModel();
    ~MS3DModel();

    IntersectionResult Intersects(const Ray &ray, float range=0.0f) const;
};


#endif
