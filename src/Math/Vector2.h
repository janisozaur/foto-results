#ifndef __VECTOR2_H_
#define __VECTOR2_H_

#include <cmath>
#include <QDebug>

class Vector2
{
public:
	float x;
	float y;

	Vector2(void):x(0.0f), y(0.0f)	{}

        Vector2(float newX, float newY):x(newX), y(newY){}

        Vector2(const float * pF):x(*pF), y((*pF)+1){}

        Vector2(const Vector2 &vec):x(vec.x), y(vec.y){}

        ~Vector2(){}

        inline void Set(float newX, float newY) {
            x=newX;
            y=newY;
	}
	
        inline void SetX(float newX) {
            x = newX;
        }
        inline void SetY(float newY){
            y = newY;
        }

        inline float GetX() const {
            return x;
        }
        inline float GetY() const {
            return y;
        }

        inline void Zero(void){
            x=y=0.0f;
        }
        inline void One(void){
            x=y=1.0f;
        }
	
	void Normalize();

	Vector2 GetNormalized() const;
	
        float GetLength() const;
	
        float GetSquaredLength() const;

        Vector2 Lerp(const Vector2 &v2, float factor) const;

        Vector2 QuadraticInterpolate(const Vector2 &v2, const Vector2 &v3, float factor) const;

        Vector2 operator+(const Vector2 &vec) const;

        Vector2 operator-(const Vector2 &vec) const;

        Vector2 operator*(const float f) const;

        Vector2 operator*(const Vector2 &vec) const;
	
        Vector2 operator/(const float f) const;

	friend Vector2 operator*(float f, const Vector2 &vec);

        bool operator==(const Vector2 &vec) const;

        bool operator!=(const Vector2 &vec) const;

        Vector2& operator+=(const Vector2 &vec);

        Vector2& operator-=(const Vector2 &vec);

        Vector2& operator*=(const float f);

        Vector2& operator*=(const Vector2 &vec);

        Vector2& operator/=(const float f);

        Vector2 operator-(void) const;

        Vector2 operator+(void) const;

        operator float* () const {
            return (float*) this;
        }

        operator const float* () const {
            return (const float*) this;
        }
};

QDebug operator<<(QDebug dbg, const Vector2 &v);
#endif
