#include "Vector2.h"

void Vector2::Normalize() {
    float length;
    float scalefactor;
    length=GetLength();

    if(length==1 || length==0)
        return;

    scalefactor = 1.0f/length;
    x *= scalefactor;
    y *= scalefactor;
}

Vector2 Vector2::GetNormalized() const {
    Vector2 result(*this);

    result.Normalize();

    return result;
}

float Vector2::GetLength() const {
    return (float)sqrt((x*x)+(y*y));
}

float Vector2::GetSquaredLength() const {
    return (x*x)+(y*y);
}

Vector2 Vector2::Lerp(const Vector2 &v2, float factor) const {
    return (*this)*(1.0f-factor) + v2*factor;
}

Vector2 Vector2::QuadraticInterpolate(const Vector2 &v2,
                                      const Vector2 &v3, float factor) const {
    return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
}

Vector2 Vector2::operator+(const Vector2 &vec) const {
    return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(const Vector2 &vec) const {
    return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator*(const float f) const {
    return Vector2(x*f, y*f);
}

Vector2 Vector2::operator*(const Vector2 &vec) const {
    return Vector2(x*vec.x, y*vec.y);
}

Vector2 Vector2::operator/(const float f) const {
    return (f==0) ? Vector2(0.0f, 0.0f) : Vector2(x/f, y/f);
}

bool Vector2::operator==(const Vector2 &vec) const {
    if(x==vec.x && y==vec.y)
        return true;
    return false;
}

bool Vector2::operator!=(const Vector2 &vec) const {
    return !((*this)==vec);
}

Vector2& Vector2::operator+=(const Vector2 &vec) {
    x+=vec.x;
    y+=vec.y;
    return (*this);
}

Vector2& Vector2::operator-=(const Vector2 &vec) {
    x-=vec.x;
    y-=vec.y;
    return (*this);
}

Vector2& Vector2::operator*=(const float f) {
    x*=f;
    y*=f;
    return (*this);
}

Vector2& Vector2::operator*=(const Vector2 &vec) {
    x*=vec.x;
    y*=vec.y;
    return (*this);
}

Vector2& Vector2::operator/=(const float f) {
    if(f!=0.0f) {
        x/=f;
        y/=f;
    }
    return (*this);
}

Vector2 operator*(float f, const Vector2 &vec) {
    return vec*f;
}

Vector2 Vector2::operator-(void) const {
    return Vector2(-x, -y);
}

Vector2 Vector2::operator+(void) const {
    return *this;
}

QDebug operator<<(QDebug dbg, const Vector2 &v) {
    dbg.nospace() << "(" << v.x << ", " << v.y << ")";
    return dbg.space();
}
