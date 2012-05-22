#ifndef __VECTOR4_H__
#define __VECTOR4_H__
#include "Vector3.h"
#include <QDebug>

class Vector4
{
public:
    float x;
    float y;
    float z;
    float w;

    Vector4(void):x(0.0f), y(0.0f), z(0.0f), w(0.0f){}

    Vector4(float newX, float newY, float newZ, float newW)
            :x(newX), y(newY), z(newZ), w(newW){}

    Vector4(const float * vec):x(*vec), y(*(vec+1)), z(*(vec+2)), w(*(vec+3)){}

    Vector4(const Vector4 &vec):x(vec.x), y(vec.y), z(vec.z), w(vec.w){}

    Vector4(const Vector3 &vec):x(vec.x), y(vec.y), z(vec.z), w(1.0f){}

    Vector4(const Vector3 &vec, float _w):x(vec.x), y(vec.y), z(vec.z), w(_w){}

    ~Vector4() {}

    inline void Set(float newX, float newY, float newZ, float newW) {
        x=newX;
        y=newY;
        z=newZ;
        w=newW;
    }
	
    inline void SetX(float newX) {x = newX;}
    inline void SetY(float newY) {y = newY;}
    inline void SetZ(float newZ) {z = newZ;}
    inline void SetW(float newW) {w = newW;}

    inline float GetX() const {return x;}
    inline float GetY() const {return y;}
    inline float GetZ() const {return z;}
    inline float GetW() const {return w;}

    inline void Zero(void) {
        x=0.0f; y=0.0f; z=0.0f; w=0.0f;
    }

    inline void One(void) {
        x=1.0f; y=1.0f; z=1.0f; w=1.0f;
    }

    float DotProduct(const Vector4 & vec);


    Vector4 Lerp(const Vector4 &v2, float factor) const;

    Vector4 QuadraticInterpolate(const Vector4 &v2, const Vector4 &v3, float factor) const;

    Vector4 operator+(const Vector4 &vec) const;

    Vector4 operator-(const Vector4 &vec) const;

    Vector4 operator*(const float f) const;

    Vector4 operator *(const Vector4 &vec) const;

    Vector4 operator/(const float f) const;

    friend Vector4 operator*(float scaleFactor, const Vector4 &vec);

    bool operator==(const Vector4 &vec) const;

    bool operator!=(const Vector4 &vec) const;

    Vector4& operator+=(const Vector4 &vec);

    Vector4& operator-=(const Vector4 &vec);

    Vector4& operator*=(const float f);

    Vector4& operator*=(const Vector4 &vec);

    Vector4& operator/=(const float f);

    Vector4 operator-(void) const {return Vector4(-x, -y, -z, -w);}

    Vector4 operator+(void) const {return (*this);}

    operator float* () const {return (float*) this;}
    operator const float* () const {return (const float*) this;}

    operator Vector3();

    void RotateX(double angle);
    Vector4 GetRotatedX(double angle) const;

    void RotateY(double angle);
    Vector4 GetRotatedY(double angle) const;

    void RotateZ(double angle);
    Vector4 GetRotatedZ(double angle) const;

    void RotateAxis(double angle, const Vector3 &axis);
    Vector4 GetRotatedAxis(double angle, const Vector3 &axis) const;
};

QDebug operator<<(QDebug dbg, const Vector4 &v);

#endif
