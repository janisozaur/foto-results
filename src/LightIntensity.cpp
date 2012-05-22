#include "LightIntensity.h"

LightIntensity LightIntensity::operator+(const LightIntensity& rhs) const {
    return LightIntensity(r+rhs.r, g+rhs.g, b+rhs.b);
}

LightIntensity LightIntensity::operator-(const LightIntensity& rhs) const {
    return LightIntensity(r-rhs.r, g-rhs.g, b-rhs.b);
}

LightIntensity LightIntensity::operator*(float num) const {
    return LightIntensity(r*num, g*num, b*num);
}

LightIntensity LightIntensity::operator*(const LightIntensity& rhs) const {
    return LightIntensity(r*rhs.r, g*rhs.g, b*rhs.b);
}

LightIntensity LightIntensity::operator/(float num) const {
    return LightIntensity(r/num, g/num, b/num);
}

LightIntensity& LightIntensity::operator+=(const LightIntensity& rhs) {
    r+=rhs.r;
    g+=rhs.g;
    b+=rhs.b;

    return (*this);
}

LightIntensity& LightIntensity::operator-=(const LightIntensity& rhs) {
    r-=rhs.r;
    g-=rhs.g;
    b-=rhs.b;

    return (*this);
}

LightIntensity& LightIntensity::operator*=(const LightIntensity& rhs) {
    r*=rhs.r;
    g*=rhs.g;
    b*=rhs.b;

    return (*this);
}

LightIntensity& LightIntensity::operator*=(float num) {
    r*=num;
    g*=num;
    b*=num;

    return (*this);
}

LightIntensity& LightIntensity::operator/=(float num) {
    r/=num;
    g/=num;
    b/=num;

    return (*this);
}

LightIntensity operator*(float num, const LightIntensity& li) {
    return LightIntensity(num*li.r, num*li.g, num*li.b);
}
bool LightIntensity::operator ==(const LightIntensity& rhs) const {
    if(r == rhs.r && g==rhs.g && b==rhs.b)
        return true;
    return false;
}
