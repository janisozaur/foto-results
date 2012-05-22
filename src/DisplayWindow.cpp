#include "DisplayWindow.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
    QWidget(parent)
{
    image=0;
    pixmap=0;
}

void DisplayWindow::setImage(Image *img) {
    if(image)
        delete pixmap;

    image=img;
    pixmap = new QImage(img->GetWidth(), img->GetHeight(), QImage::Format_RGB32);
    setMinimumSize(img->GetWidth(), img->GetHeight());
}

void DisplayWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);


    if(image) {
        QRgb* pixels = (QRgb*)pixmap->bits();
        for(unsigned int i=0;i<image->GetWidth()*image->GetHeight();i++) {
            LightIntensity p = image->GetPixel(i%image->GetWidth(), i/image->GetWidth());
            p*=255;
            if(p.r>255)p.r=255;
            if(p.g>255)p.g=255;
            if(p.b>255)p.b=255;
            pixels[i] = qRgb(p.r, p.g, p.b);
        }
        p.drawImage(0, 0, *pixmap);
    }
    else {
        p.fillRect(rect(), Qt::black);
    }
}
