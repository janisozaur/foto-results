#include "Texture.h"

Texture::Texture() {
    image=0;
    width=0;
    height=0;
}

Texture::Texture(const char* name) {
    image=0;
    Load(name);
}

Texture::Texture(unsigned int w, unsigned int h) {
    image = new QImage(w, h, QImage::Format_RGB32);
    width = w;
    height = h;
}


void Texture::Load(const char* name) {
    if(image)
        delete image;
    image = new QImage(name);
    width = image->width();
    height = image->height();
}

Texture::~Texture() {
    if(image)
        delete image;
}

Color Texture::Sample(float u, float v) {
    if(image) {
        int x, y;
        x = u*width;
        y = v*height;

        QRgb pixel = image->pixel(x,y);
        Color result(float(qRed(pixel))/255,
                     float(qGreen(pixel))/255,
                     float(qBlue(pixel))/255);

        return result;
    }
    return Color();
}

Color Texture::SampleSpherical(const Vector3 &pos) {
    float theta = acos(pos.y);
    float phi = atan2(pos.x, pos.z);
    if(phi<0.0) {
        phi += 2*M_PI;
    }

    float u = phi*(1.0f/(2*M_PI));
    float v = 1.0f - theta * (1.0f/M_PI);

    return Sample(1.0f-u,1.0f-v);
}

void Texture::SetPixel(unsigned x, unsigned y, Color color) {
    image->setPixel(x,y, qRgb(color.r*255, color.g*255, color.b*255));
}

void Texture::SaveToFile(QString filename) {
    image->save(filename);
}


float Texture::GetWhiteToBlackPixelRatio() const {
    float all = width*height;
    float white = 0;

    for(int i=0;i<width*height;i++) {
        if(image->pixel(i%width, i/width)==qRgb(255,255,255))
            white++;
    }

    return white/all;
}
