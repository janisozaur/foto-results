#include <cstdio>
#include "MS3DModel.h"
#include <string.h>
#include <QDebug>
#include "Triangle.h"
#include "Box.h"
#define PI 3.14156
#include <cfloat>

MS3DModel::MS3DModel()
{
	numMaterials=0;
	numMeshes=0;
	numTriangles=0;
	numVertices=0;
	numJoints = 0;

	vertices=NULL;
	triangles=NULL;
	meshes=NULL;
	materials=NULL;

        position.Zero();
        scale = 1;
}

MS3DModel::~MS3DModel()
{
	int i;
	for(i=0;i<numMeshes;i++)delete[] meshes[i].trianglesIndices;


	numMeshes=0;
	if(meshes!=NULL)
	{
		delete[] meshes;
		meshes=NULL;
	}

	numMaterials=0;
	if(materials!=NULL)
	{
		delete[] materials;
		materials=NULL;
	}

	numTriangles=0;
	if(triangles!=NULL)
	{
		delete[] triangles;
		triangles=NULL;
	}

	numVertices=0;
	if(vertices!=NULL)
	{
		delete[] vertices;
		vertices=NULL;
	}

}



void MS3DModel::Load(const char* filename)
{
	FILE *pf;
	int i;
	char tempChar;
	float tempFloat;
	int tempInt;
	char tempString[40];
	pf = fopen(filename, "rb");
	
	if(!pf)
	{
            qDebug()<<"no file";
            //		MessageBox(NULL,"Nie mo¿na za³adowaæ pliku", "B³¹d", MB_OK);
		return;
	}

	//Read ms3d header
	//====================
	char id[11];
	int version;
	memset(id, '\0', 11);
	fread(id, sizeof(char), 10, pf);
	fread(&version, sizeof(int), 1, pf);

	if(strcmp(id, "MS3D000000")!=0 || version!=4)
	{
                //MessageBox(NULL,"To nie jest plik modelu milkshape", "B³¹d", MB_OK);
		return;
	}
	//====================

	//Read vertices data
	//====================
	fread(&numVertices, sizeof(short), 1,pf);
	
        vertices = new MS3DVertex[numVertices];

	
	for(i=0;i<numVertices;i++)
	{
		fread(tempString, sizeof(char), 1, pf); //flags, we're not interested
		fread(vertices[i].position, sizeof(float), 3, pf);
		fread(&vertices[i].boneIndex, sizeof(char), 1, pf);
		fread(tempString, sizeof(char), 1, pf); //reference count, we're not interested 
	}
	//====================

	//Read triangles data
	//====================
	fread(&numTriangles, sizeof(short), 1, pf);

        triangles = new MS3DTriangle[numTriangles];
	

	for(i=0;i<numTriangles;i++)
	{
		fread(tempString, sizeof(short), 1, pf); //flags
		fread(triangles[i].vertexIndices, sizeof(short), 3, pf);
		fread(triangles[i].vertexNormal, sizeof(float), 9, pf);
		fread(triangles[i].s, sizeof(float), 3, pf);
		fread(triangles[i].t, sizeof(float), 3, pf);
		fread(tempString, sizeof(char), 1, pf); //smoothing group
		fread(&triangles[i].groupIndex, sizeof(char), 1, pf);
	}
	//====================

	//Read meshes data
	//====================
	fread(&numMeshes, sizeof(short), 1, pf);
        meshes = new MS3DMesh[numMeshes];

	for(i=0;i<numMeshes;i++)
	{
		fread(tempString, sizeof(char), 1, pf);
		fread(tempString, sizeof(char), 32, pf);
		fread(&meshes[i].numTriangles, sizeof(short), 1, pf);
		meshes[i].trianglesIndices = new unsigned short[meshes[i].numTriangles];
		fread(meshes[i].trianglesIndices, sizeof(short), meshes[i].numTriangles, pf);
		fread(&meshes[i].materialIndex, sizeof(char), 1, pf);
	}
	//====================

	//Read materials data
	//====================
	fread(&numMaterials, sizeof(short), 1, pf);
        materials = new MS3DMaterial[numMaterials];

	for(i=0;i<numMaterials;i++)
	{
		fread(tempString, sizeof(char), 32, pf);
		fread(materials[i].ambient, sizeof(float), 4, pf);
		fread(materials[i].diffuse, sizeof(float), 4, pf);
		fread(materials[i].specular, sizeof(float), 4, pf);
		fread(materials[i].emissive, sizeof(float), 4, pf);
		fread(&materials[i].shinniness, sizeof(float), 1, pf);
		fread(&materials[i].transparency, sizeof(float), 1, pf);
		fread(tempString, sizeof(char), 1, pf); //mode
		fread(materials[i].texture, sizeof(char), 128, pf);
		fread(materials[i].alphamap, sizeof(char), 128, pf);


	}

        bboxMin = Vector3(5000,5000,5000);
        bboxMax = Vector3(-5000,-5000,-5000);
        for(int i=0;i<numVertices;i++) {
            if(vertices[i].position[0]>bboxMax.x)bboxMax.x = vertices[i].position[0];
            if(vertices[i].position[1]>bboxMax.y)bboxMax.y = vertices[i].position[1];
            if(vertices[i].position[2]>bboxMax.z)bboxMax.z = vertices[i].position[2];

            if(vertices[i].position[0]<bboxMin.x)bboxMin.x = vertices[i].position[0];
            if(vertices[i].position[1]<bboxMin.y)bboxMin.y = vertices[i].position[1];
            if(vertices[i].position[2]<bboxMin.z)bboxMin.z = vertices[i].position[2];
        }

	fclose(pf);
       // qDebug()<<"ok"<<bboxMin<<bboxMax;
}

void MS3DModel::SetPositionAndScale(const Vector3 &position, float scale) {
    this->position = position;
    this->scale = scale;
}

IntersectionResult MS3DModel::Intersects(const Ray &ray, float range) const {

    //calculate model's transformed bounding box
    Vector3 bboxMinTransformed = scale*bboxMin+position;
    Vector3 bboxMaxTransformed = scale*bboxMax+position;
    Vector3 bboxCenter = (bboxMinTransformed+ bboxMaxTransformed)/2;

    Box boundingBox(bboxCenter, 2*(bboxMaxTransformed - bboxCenter));

    IntersectionResult closest;
    float closestDist = FLT_MAX;

    //chec first if we intersects with bounding box
    if(boundingBox.Intersects(ray, range).type!=MISS) {
        for(int i=0; i<numMeshes;i++)
        {
            for(int j=0;j<meshes[i].numTriangles;j++)
            {
                //qDebug()<<j;
                int triangleIndex = meshes[i].trianglesIndices[j];
                const MS3DTriangle* tri = &triangles[triangleIndex];


                Triangle t(scale*Vector3(vertices[tri->vertexIndices[0]].position)+position,
                           scale*Vector3(vertices[tri->vertexIndices[1]].position)+position,
                           scale*Vector3(vertices[tri->vertexIndices[2]].position)+position,
                           Vector3(tri->vertexNormal[0]),
                           Vector3(tri->vertexNormal[1]),
                           Vector3(tri->vertexNormal[2]));


                IntersectionResult ir = t.Intersects(ray, range);
                if(ir.type!=MISS && ir.distance < closestDist) {
                    closest = ir;
                    closestDist = ir.distance;
                }
            }
        }
    }

        closest.object = this;
    return closest;
}

