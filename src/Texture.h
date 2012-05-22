#ifndef TEXTURE_H
#define TEXTURE_H
#include <QImage>
#include "LightIntensity.h"
#include "Math/Vector3.h"

class Texture
{
public:
    Texture();
    Texture(unsigned int w, unsigned int h);
    Texture(const char* name);
    ~Texture();

    void Load(const char* name);

    unsigned int GetWidth() const {
        return width;
    }

    unsigned int GetHeight() const {
        return height;
    }

    Color Sample(float u, float v);
    Color SampleSpherical(const Vector3& pos);

    void SetPixel(unsigned x, unsigned y, Color color);

    void SaveToFile(QString filename);

    float GetWhiteToBlackPixelRatio() const;

private:
    QImage* image;

    unsigned int width;
    unsigned int height;
};

#endif // TEXTURE_H
