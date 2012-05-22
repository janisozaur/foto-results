#ifndef LIGHTINTENSITY_H
#define LIGHTINTENSITY_H

/**
  Defines light intensity (equivalent to color) with all operations.
  */
class LightIntensity
{
public:
    LightIntensity() {
        r=g=b=0;
    }

    LightIntensity(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    LightIntensity(double r, double g) {
        this->r = r;
        this->g = g;
        this->b = 0;
    }

    LightIntensity(double r) {
        this->r = r;
        g=b=0;
    }

    LightIntensity operator+(const LightIntensity& rhs) const;
    LightIntensity operator-(const LightIntensity& rhs) const;
    LightIntensity operator*(const LightIntensity& rhs) const;
    LightIntensity operator*(float num) const;
    LightIntensity operator/(float num) const;
    bool operator==(const LightIntensity& rhs) const;

    LightIntensity& operator+=(const LightIntensity& rhs);
    LightIntensity& operator-=(const LightIntensity& rhs);
    LightIntensity& operator*=(const LightIntensity& rhs);
    LightIntensity& operator*=(float num);
    LightIntensity& operator/=(float num);

    friend LightIntensity operator*(float num, const LightIntensity& li);


    double r;
    double g;
    double b;
};

typedef LightIntensity Color;
#endif // LIGHTINTENSITY_H
