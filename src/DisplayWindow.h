#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <QtGui>
#include <QTimer>
#include "Image.h"

/**
  Window that displays rendered image.
  */
class DisplayWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWindow(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void setImage(Image* img);

private:
    Image* image;
    QImage* pixmap;
};

#endif // DISPLAYWINDOW_H
