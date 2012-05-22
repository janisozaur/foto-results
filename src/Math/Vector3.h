#ifndef __VECTOR3_H__
#define __VECTOR3_H__
#include <cmath>
#include <QDebug>

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3(void):x(0.0f), y(0.0f), z(0.0f)	{}

	Vector3(float newX, float newY, float newZ):x(newX), y(newY), z(newZ){}

        Vector3(const float * pft):x(*pft), y(*(pft+1)), z(*(pft+2)) {}

        Vector3(const Vector3 &vec):x(vec.x), y(vec.y), z(vec.z) {}

	~Vector3() {}

        inline void Set(float newX, float newY, float newZ) {
            x=newX;
            y=newY;
            z=newZ;
	}
	
        inline void SetX(float newX) {
            x = newX;
        }
        inline void SetY(float newY) {
            y = newY;
        }
        inline void SetZ(float newZ) {
            z = newZ;
        }

        float GetX() const {
            return x;
        }
        float GetY() const {
            return y;
        }
        float GetZ() const {
            return z;
        }

	void Zero(void)	{
            x=y=z=0.0f;
	}

        void One(void) {
            x=y=z=1.0f;
	}
	
	Vector3 CrossProduct(const Vector3 &vec) const;

        float DotProduct(const Vector3 &vec) const;

        float AngleWith(const Vector3 &vec) const;
	
	void Normalize();

	Vector3 GetNormalized() const;
	
	float GetLength() const;
		
        float GetSquaredLength() const;

        Vector3 Lerp(const Vector3 &v2, float factor) const;

        Vector3 QuadraticInterpolate(const Vector3 &v2, const Vector3 &v3, float factor) const;

        Vector3 operator+(const Vector3 &vec) const;
	
        Vector3 operator-(const Vector3 &vec) const;

        Vector3 operator*(const float f) const;

        Vector3 operator*(const Vector3& vec) const;
	
        Vector3 operator/(const float f) const;

	friend Vector3 operator*(float f, const Vector3 &vec);

	bool operator==(const Vector3 &vec) const;

        bool operator!=(const Vector3 &vec) const;

        Vector3& operator+=(const Vector3 &vec);

        Vector3& operator-=(const Vector3 &vec);

        Vector3& operator*=(const float f);

        Vector3& operator*=(const Vector3& vec);
	
        Vector3& operator/=(const float f);

	Vector3 operator-(void) const {return Vector3(-x, -y, -z);}
	Vector3 operator+(void) const {return *this;}

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

        void RotateX(double angle);
        Vector3 GetRotatedX(double angle) const;

        void RotateY(double angle);
        Vector3 GetRotatedY(double angle) const;

        void RotateZ(double angle);
        Vector3 GetRotatedZ(double angle) const;

        void RotateAxis(double angle, const Vector3 &axis);
        Vector3 GetRotatedAxis(double angle, const Vector3 &axis) const;

        void PackTo01();
        Vector3 GetPackedTo01() const;

        Vector3 Reflect(const Vector3& normal) const;

        Vector3 Refract(const Vector3& normal, float eta) const;
};

QDebug operator<<(QDebug dbg, const Vector3 &v);

#endif
