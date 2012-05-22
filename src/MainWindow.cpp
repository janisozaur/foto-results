#include "MainWindow.h"
#include "Geometry/Sphere.h"
#include "Scripts/ScriptManager.h"
#include "Lights/AreaLight.h"
#include "Geometry/CornellBox.h"
#include "omp.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

    setlocale(LC_ALL,"C");
    omp_set_dynamic(1);

    script = new Script();

    display.resize(640,480);
    layout = new QHBoxLayout;
    layout->addWidget(&display);

    scriptSource = new QTextEdit();
    QFile scriptFile("script.txt");
    if(scriptFile.open(QFile::ReadOnly)) {
        QString src = QString(scriptFile.readAll());
        scriptSource->setText(src);

        scriptFile.close();
    }
    else {
        scriptSource->setText("render = function(image) \n"
                              "camera = Camera(Vector3(0.0,0.0,-20.0), Vector3(0.0,0.0,0.0), 0.01, 500.0, 45.0); \n"
                              "camera:SetImage(image); \n"
                              "camera:Recalculate(); \n"
                              "scene = Scene(); \n"
                              "sphere1 = Sphere(Vector3(3,0,0), 5.0); \n"
                              "sphere2 = Sphere(Vector3(-6,-2,0), 5.0); \n"
                              "mat = Material(Color(0.1, 0.1, 0.1), Color(1,1,1), Color(1,1,1), 5, 0); \n"
                              "sphere1:SetMaterial(mat); \n"
                              "sphere2:SetMaterial(mat); \n"
                              "scene:AddLight(PointLight(Vector3(0, 40, -20), Color(1.0, 0.0, 0.95), Vector3(1,0,0))); \n"
                              "scene:AddLight(AmbientLight(Color(0.1,0.1,0.1))); \n"
                              "scene:AddGeometry(sphere1); \n"
                              "scene:AddGeometry(sphere2); \n"
                              "camera:RenderScene(scene); \n"
                              "end");
    }
    scriptSource->setMinimumWidth(600);
    layout->addWidget(scriptSource);

    mainLayout = new QVBoxLayout;
    renderButton = new QPushButton("Render");
    mainLayout->addLayout(layout);
    mainLayout->addWidget(renderButton);

    setLayout(mainLayout);

    image = new Image(640, 480);
    display.setImage(image);




    connect(renderButton, SIGNAL(clicked()), this, SLOT(renderScene()));
}

MainWindow::~MainWindow() {
    QFile scriptFile("script.txt");
    scriptFile.open(QFile::WriteOnly);
    scriptFile.write(scriptSource->toPlainText().toAscii());
    scriptFile.close();


    delete image;
    image=0;
    delete script;
}

void MainWindow::renderScene() {
    if(script->Run(scriptSource->toPlainText()))
        script->callFunction("render", image);

    repaint();
}

