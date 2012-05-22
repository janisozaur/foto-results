#include <QApplication>
#include <QDebug>
#include "MainWindow.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Ray.h"
#include "Math/Plane.h"
#include "Geometry/Sphere.h"
#include "Geometry/Triangle.h"
#include "Geometry/CornellBox.h"
#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Lights/PointLight.h"
#include "Lights/AreaLight.h"
#include "Lights/AmbientLight.h"
#include "Scene.h"
#include "Script.h"
#include "ScriptManager.h"
#include "Geometry/MS3DModel.h"
#include "Geometry/Box.h"

void bindToLua() {
    lua_State* L =  ScriptManager::Get()->GetState();

    luabind::module(L)
    [
        luabind::class_<Vector2>("Vector2")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float>())
            .def(luabind::constructor<const Vector2&>())
    ];

    luabind::module(L)
    [
        luabind::class_<Vector3>("Vector3")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float>())
            .def(luabind::constructor<const Vector3&>())
    ];


    luabind::module(L)
    [
        luabind::class_<Vector4>("Vector4")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float, float>())
            .def(luabind::constructor<const Vector4&>())
            .def(luabind::constructor<const Vector3&>())
            .def(luabind::constructor<const Vector3&, float>())
    ];


    luabind::module(L)
    [
        luabind::class_<Color>("Color")
            .def(luabind::constructor<>())
            .def(luabind::constructor<double, double, double>())
            .def(luabind::constructor<double, double>())
            .def(luabind::constructor<double>())
    ];

    luabind::module(L)
    [
        luabind::class_<AmbientLight>("AmbientLight")
            .def(luabind::constructor<>())
            .def(luabind::constructor<Color>())
    ];

    luabind::module(L)
    [
        luabind::class_<PointLight, AmbientLight>("PointLight")
            .def(luabind::constructor<>())
            .def(luabind::constructor<Vector3, Color, Vector3>())
    ];

    luabind::module(L)
    [
        luabind::class_<AreaLight, AmbientLight>("AreaLight")
            .def(luabind::constructor<Vector3, Vector2, Color, Vector3, int>())
    ];


    luabind::module(L)
    [
        luabind::class_<Texture>("Texture")
            .def(luabind::constructor<const char*>())
    ];

    luabind::module(L)
    [
        luabind::class_<Material>("Material")
            .def("SetTexture", &Material::SetTexture)
    ];



    luabind::module(L)
    [
        luabind::class_<DiffuseMaterial, Material>("DiffuseMaterial")
            .def(luabind::constructor<const Color&, const Color&, const Color&, float, float>())
            .def(luabind::constructor<const Color&, const Color&, const Color&, float, float, Texture*>())
    ];

    luabind::module(L)
    [
        luabind::class_<ReflectiveMaterial, Material>("ReflectiveMaterial")
            .def(luabind::constructor<>())
            .def(luabind::constructor<Texture*>())
    ];

    luabind::module(L)
    [
        luabind::class_<RefractiveMaterial, Material>("RefractiveMaterial")
            .def(luabind::constructor<float>())
            .def(luabind::constructor<float, Texture*>())
    ];


    luabind::module(L)
    [
        luabind::class_<Geometry>("Geometry")
            .def("SetMaterial", &Geometry::SetMaterial)
    ];


    luabind::module(L)
    [
        luabind::class_<Sphere, Geometry>("Sphere")
            .def(luabind::constructor<>())
            .def(luabind::constructor<Vector3, float>())
    ];

    luabind::module(L)
    [
        luabind::class_<CornellBox, Geometry>("CornellBox")
            .def(luabind::constructor<Vector3, Vector3>())
    ];

    luabind::module(L)
    [
        luabind::class_<Box, Geometry>("Box")
            .def(luabind::constructor<Vector3, Vector3>())
    ];

    luabind::module(L)
    [
        luabind::class_<MS3DModel, Geometry>("MS3DModel")
            .def(luabind::constructor<>())
            .def("Load", &MS3DModel::Load)
            .def("SetPosScale", &MS3DModel::SetPositionAndScale)
    ];

    luabind::module(L)
    [
        luabind::class_<Scene>("Scene")
            .def(luabind::constructor<>())
            .def("AddGeometry", &Scene::AddGeometry, luabind::adopt(_2))
            .def("AddLight", &Scene::AddLight, luabind::adopt(_2))
    ];

    luabind::module(L)
    [
        luabind::class_<Image>("Image")
    ];

    luabind::module(L)
    [
        luabind::class_<Camera>("Camera")
            .def(luabind::constructor<>())
            .def(luabind::constructor<Vector3, Vector3, float, float, float>())
            .def("SetImage", &Camera::SetImage)
            .def("RenderScene", (void (Camera::*)(Scene*, unsigned int))&Camera::RenderScene)
            .def("RenderScene", (void (Camera::*)(Scene*, unsigned int, unsigned int, unsigned int))&Camera::RenderScene)
            .def("RenderSceneStream", &Camera::RenderSceneStream)
            .def("VisualizePhotonMap", &Camera::VisualizePhotonMap)
            .def("VisualizeStreamPhotonMap", &Camera::VisualizeStreamPhotonMap)
            .def("SetResultFileName", &Camera::SetResultFileName)
            .def("SetPhotonMappingParams", &Camera::SetPhotonMappingParams)
    ];
}




QString GenerateScript(int numGlobalPhotons, int numCausticPhotons, int numAssociatedPhotons, float radius, QString resultFileName) {
    QString scriptSrc = QString("render = function(image)\n") +
                        QString("camera = Camera(Vector3(0.0,0.0,-30.0), Vector3(0.0,0.0,0.0), 0.01, 500.0, 55.0);\n")+
                        QString("camera:SetImage(image);\n")+
                        QString("camera:SetResultFileName(\"" + resultFileName + "\");\n")+
                        QString("camera:SetPhotonMappingParams(5, 400, 3,  100);\n")+
                        QString("scene = Scene();\n")+
                        QString("sphere1 = Sphere(Vector3(-3,-7,2), 3);\n")+
                        QString("sphere2 = Sphere(Vector3(3,-7, -5), 3);\n")+
                        QString("sphere3 = Sphere(Vector3(-4,-7,-6), 3);\n")+
                        QString("model = MS3DModel();\n")+
                        QString("model:Load(\"suzanne.ms3d\");\n")+
                        QString("mat = ReflectiveMaterial();\n")+
                        QString("mat2 = RefractiveMaterial(0.45);\n")+
                        QString("mat3 = DiffuseMaterial(Color(0.1,0.1,0.1), Color(1,1,1), Color(1,1,1), 15, 0.9);\n")+
                        QString("sphere1:SetMaterial(mat);\n")+
                        QString("sphere2:SetMaterial(mat2);\n")+
                        QString("sphere3:SetMaterial(mat3);\n")+
                        QString("model:SetMaterial(mat3);\n")+
                        QString("model:SetPosScale(Vector3(1,-2,3), 4);\n")+
                        QString("scene:AddLight(PointLight(Vector3(0, 9.5, 0), Color(1.2,1.2,1.2), Vector3(1,0.1,0.0)));\n")+
                        QString("scene:AddGeometry(CornellBox(Vector3(0,0,0), Vector3(20,20,20)));\n")+
                        QString("scene:AddGeometry(sphere1);\n")+
                        QString("scene:AddGeometry(sphere2);\n")+
                        QString("scene:AddGeometry(model);\n")+
                        QString("camera:RenderSceneStream(scene,1," + QString::number(numGlobalPhotons) +","+QString::number(numCausticPhotons) + "," + QString::number(numAssociatedPhotons) + "," + QString::number((float)radius/10) + ");\n")+
                        QString("end");


    qDebug()<<scriptSrc;
        return scriptSrc;
            /*(--camera:RenderScene(scene, 1);
            --camera:RenderScene(scene,1, 12000, 5000);
            --camera:VisualizePhotonMap(scene, 25000, 6);
            --camera:VisualizeStreamPhotonMap(scene, 12000, 6, 50, 0.5);*/

}

int main(int argc, char *argv[])
{


    ScriptManager scriptManager;
    scriptManager.Initialize();
    bindToLua();

    int res;

    if(argc>1) {
        QCoreApplication a(argc, argv);

        setlocale(LC_ALL,"C");
        //omp_set_dynamic(1);
        Script s;
/*        QFile scriptFile(a.arguments().last());
        if(scriptFile.open(QFile::ReadOnly)) {
        */
        QString scriptSrc = GenerateScript(QString(argv[1]).toInt(),  QString(argv[2]).toInt(),
                                           QString(argv[3]).toInt(),  QString(argv[4]).toFloat(), argv[5]);//scriptFile.readAll();

            if(s.Run(scriptSrc)) {
                Image img(640, 480);
                s.callFunction("render", &img);
            }
          //  scriptFile.close();
        //}
    }
    else {
        QApplication a(argc, argv);


        MainWindow window;
        window.show();

        res = a.exec();
    }

    scriptManager.Release();

    return res;
}

