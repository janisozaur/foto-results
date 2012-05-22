#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__
#include "Vector4.h"

class Matrix4x4
{
public:
    float entries[16];

    Matrix4x4() {}
    Matrix4x4(float e0, float e1, float e2, float e3,
              float e4, float e5, float e6, float e7,
              float e8, float e9, float e10, float e11,
              float e12, float e13, float e14, float e15);
    Matrix4x4(const float * f);
    Matrix4x4(const Matrix4x4 &mat);
    ~Matrix4x4() {}

    void SetEntry(int position, float value);
    float GetEntry(int position) const;
    Vector4 GetRow(int position) const;
    Vector4 GetColumn(int position) const;

    void SetRow(int position, const Vector4& row);
    void SetColumn(int position, const Vector4& row);
	
    void LoadIdentity();
    void LoadZero();
	
    Matrix4x4 operator+(const Matrix4x4 &mat) const;
    Matrix4x4 operator-(const Matrix4x4 &mat) const;
    Matrix4x4 operator*(const Matrix4x4 &mat) const;
    Matrix4x4 operator*(const float f) const;
    Matrix4x4 operator/(const float f) const;
    friend Matrix4x4 operator*(float scaleFactor, const Matrix4x4 &mat);

    bool operator==(const Matrix4x4 &mat) const;
    bool operator!=(const Matrix4x4 &mat) const;

    Matrix4x4& operator+=(const Matrix4x4 &mat);
    Matrix4x4& operator-=(const Matrix4x4 &mat);
    Matrix4x4& operator*=(const Matrix4x4 &mat);
    Matrix4x4& operator*=(const float f);
    Matrix4x4& operator/=(const float f);

    Matrix4x4 operator-(void) const;
    Matrix4x4 operator+(void) const {return (*this);}
	
    Vector4 operator*(const Vector4 vec) const;

    void RotateVector3D(Vector3 &vec) const {
        vec=GetRotatedVector3D(vec);
    }

    void InverseRotateVector3D(Vector3 &vec) const {
        vec=GetInverseRotatedVector3D(vec);
    }

    Vector3 GetRotatedVector3D(const Vector3 &vec) const;
    Vector3 GetInverseRotatedVector3D(const Vector3 &vec) const;

    void TranslateVector3D(Vector3 &vec) const {
        vec=GetTranslatedVector3D(vec);
    }

    void InverseTranslateVector3D(Vector3 &vec) const {
        vec=GetInverseTranslatedVector3D(vec);
    }
	
    Vector3 GetTranslatedVector3D(const Vector3 &vec) const;
    Vector3 GetInverseTranslatedVector3D(const Vector3 &vec) const;

    void Invert();
    Matrix4x4 GetInverse() const;
    void Transpose();
    Matrix4x4 GetTranspose() const;
    void InvertTranspose();
    Matrix4x4 GetInverseTranspose() const;

    void AffineInvert();
    Matrix4x4 GetAffineInverse() const;
    void AffineInvertTranspose();
    Matrix4x4 GetAffineInverseTranspose() const;

    void SetTranslation(const Vector3 &translation);
    void SetScale(const Vector3 &scaleFactor);
    void SetUniformScale(const float scaleFactor);
    void SetRotationAxis(const double angle, const Vector3 &axis);
    void SetRotationX(const double angle);
    void SetRotationY(const double angle);
    void SetRotationZ(const double angle);
    void SetRotationEuler(const double angleX, const double angleY, const double angleZ);
    void SetPerspective(float left, float right, float bottom, float top, float n, float f);
    void SetPerspective(float fovy, float aspect, float n, float f);
    void SetOrtho(float left, float right, float bottom, float top, float n, float f);

    void SetTranslationPart(const Vector3 &translation);
    void SetRotationPartEuler(const float angleX, const float angleY, const float angleZ);
    void SetRotationPartEuler(const Vector3 &rotations) {
        SetRotationPartEuler((double)rotations.x, (double)rotations.y, (double)rotations.z);
    }

    operator float* () const {return (float*) this;}
    operator const float* () const {return (const float*) this;}
};

#endif
