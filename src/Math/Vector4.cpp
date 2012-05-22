#include "Vector4.h"

inline float Vector4::DotProduct(const Vector4 & vec) {
    return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}

Vector4 Vector4::Lerp(const Vector4 &v2, float factor) const {
    return (*this)*(1.0f-factor)+v2*factor;
}

Vector4 Vector4::QuadraticInterpolate(const Vector4 &v2, const Vector4 &v3, float factor) const {
    return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
}

Vector4 Vector4::operator+(const Vector4 &vec) const {
    return Vector4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

Vector4 Vector4::operator-(const Vector4 &vec) const {
    return Vector4(x-vec.x, y-vec.y, z-vec.z, w-vec.w);
}

Vector4 Vector4::operator*(const float f) const {
    return Vector4(x*f, y*f, z*f, w*f);
}

Vector4 Vector4::operator *(const Vector4 &vec) const {
    return Vector4(x*vec.x, y*vec.y, z*vec.z, w*vec.w);
}

Vector4 Vector4::operator/(const float f) const {
        return f==0.0f ? Vector4(0.0f, 0.0f, 0.0f, 0.0f)
                       : Vector4(x/f, y/f, z/f, w/f);
}

Vector4 operator*(float scaleFactor, const Vector4 &vec) {
    return vec*scaleFactor;
}

bool Vector4::operator==(const Vector4 &vec) const {
    if(x==vec.x && y==vec.y && z==vec.z && w==vec.w)
        return true;

    return false;
}

bool Vector4::operator!=(const Vector4 &vec) const {
    return !((*this)==vec);
}

Vector4& Vector4::operator+=(const Vector4 &vec) {
    x+=vec.x; y+=vec.y; z+=vec.z; w+=vec.w;
    return (*this);
}

Vector4& Vector4::operator-=(const Vector4 &vec) {
    x-=vec.x; y-=vec.y; z-=vec.z; w-=vec.w;
    return (*this);
}

Vector4& Vector4::operator*=(const float f) {
    x*=f; y*=f; z*=f; w*=f;
    return (*this);
}

Vector4& Vector4::operator *=(const Vector4 &vec) {
    x*=vec.x;
    y*=vec.y;
    z*=vec.z;
    w*=vec.w;

    return (*this);
}

Vector4& Vector4::operator/=(const float f) {
    if(f!=0.0f) {
        x/=f; y/=f; z/=f; w/=f;
    }
    return (*this);
}

Vector4::operator Vector3() {
    if(w==0.0f || w==1.0f)
        return Vector3(x, y, z);
    else
        return Vector3(x/w, y/w, z/w);
}

void Vector4::RotateX(double angle) {
    (*this)=GetRotatedX(angle);
}

Vector4 Vector4::GetRotatedX(double angle) const {
    Vector3 v3d(x, y, z);

    v3d.RotateX(angle);

    return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateY(double angle) {
    (*this)=GetRotatedY(angle);
}

Vector4 Vector4::GetRotatedY(double angle) const {
    Vector3 v3d(x, y, z);

    v3d.RotateY(angle);

    return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateZ(double angle) {
    (*this)=GetRotatedZ(angle);
}

Vector4 Vector4::GetRotatedZ(double angle) const {
    Vector3 v3d(x, y, z);

    v3d.RotateZ(angle);

    return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateAxis(double angle, const Vector3 & axis) {
    (*this)=GetRotatedAxis(angle, axis);
}

Vector4 Vector4::GetRotatedAxis(double angle, const Vector3 & axis) const {
    Vector3 v3d(x, y, z);

    v3d.RotateAxis(angle, axis);

    return Vector4(v3d.x, v3d.y, v3d.z, w);
}

QDebug operator<<(QDebug dbg, const Vector4 &v) {
    dbg.nospace() << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return dbg.space();
}
