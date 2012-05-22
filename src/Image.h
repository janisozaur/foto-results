#ifndef IMAGE_H
#define IMAGE_H
#include "LightIntensity.h"
#include <QString>

/**
  Image into which we render.
  */
class Image
{
public:
    /**
      Creates default image of size 640x480px
      */
    Image();

    /**
      Creates image of specyfic size.
      \param w width of image
      \param h height of image
      */
    Image(unsigned int w, unsigned int h);

    /**
      Destructor
      */
    ~Image();


    /**
      Returns width of the image
      */
    unsigned int GetWidth() const {
        return width;
    }

    /**
      Returns height of the image
      */
    unsigned int GetHeight() const {
        return height;
    }

    /**
      Sets pixel on position (x, y) to light intensity value li
      \param x x position of pixel to set
      \param y y position of pixel to set
      \param li light intensity value
      */
    void SetPixel(unsigned int x, unsigned int y, const LightIntensity& li);

    /**
      Gets light intensity of pixel on position (x,y)
      \param x x position of pixel
      \param y y position of pixel
      */
    LightIntensity GetPixel(unsigned int x, unsigned int y);

    /**
      Fills all image pixels with clearColor light intensity value
      \param clearColor light intensity value to fill all pixels
      */
    void Clear(LightIntensity clearColor);

    /**
      Saves contenst of image to .png file.
      All values above 255 are clamped to 255.
      \param fileName path to file to save image (it should not contains extension to file, it is set automaticaly to png)
      */
    void SaveToFile(QString fileName);

private:
    unsigned int width;
    unsigned int height;
    LightIntensity* pixels;
};

#endif // IMAGE_H
