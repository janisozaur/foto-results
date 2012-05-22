#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "DisplayWindow.h"
#include "Scene.h"
#include "Material.h"
#include "Camera.h"
#include "Lights/PointLight.h"
#include "Scripts/Script.h"


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void renderScene();

private:
    DisplayWindow display;
    QVBoxLayout* mainLayout;
    QHBoxLayout* layout;
    QPushButton* renderButton;
    Image* image;
    QTimer timer;
    QTextEdit* scriptSource;
    Script* script;
};

#endif // MAINWINDOW_H
