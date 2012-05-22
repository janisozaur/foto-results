#include "Image.h"
#include <QImage>
#include <QDebug>

Image::Image()
{
    width = 640;
    height = 480;
    pixels = new LightIntensity[640*480];
}

Image::Image(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    pixels = new LightIntensity[w*h];
}

Image::~Image() {
    delete[] pixels;
}

void Image::SetPixel(unsigned int x, unsigned int y, const LightIntensity &li) {
    if(x<width && y<height) {
        pixels[width*y+x] = li;
    }
}

LightIntensity Image::GetPixel(unsigned int x, unsigned int y) {
    return pixels[y*width+x];
}

void Image::Clear(LightIntensity clearColor) {
    for(unsigned int i=0;i<width*height;i++)
        pixels[i] = clearColor;
}

void Image::SaveToFile(QString fileName) {

    QImage resultImage(width, height, QImage::Format_RGB32);
    QRgb* p = (QRgb*)resultImage.bits();

    for(unsigned int i=0;i<width*height;i++) {
        unsigned int r = pixels[i].r*255;
        unsigned int g = pixels[i].g*255;
        unsigned int b = pixels[i].b*255;

        if(r>255)r=255;
        if(g>255)g=255;
        if(b>255)b=255;


        *p = qRgb(r, g, b);
        p++;
    }

    resultImage.save(QString(fileName + ".png"));
}
