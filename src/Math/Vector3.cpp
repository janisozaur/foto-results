#include "Vector3.h"

float Vector3::GetLength() const {
    return (float)sqrt(x*x + y*y + z*z);
}

float Vector3::GetSquaredLength() const {
    return (x*x)+(y*y)+(z*z);
}

void Vector3::Normalize() {
    float f = (float)sqrt(x*x + y*y + z*z);
    if(f != 0.0f && f!=1.0f) {
        x/=f;
        y/=f;
        z/=f;
    }
}


Vector3 Vector3::GetNormalized() const {
    Vector3 result(*this);

    result.Normalize();

    return result;
}

float Vector3::DotProduct(const Vector3 &vec) const {
    return x*vec.x + y*vec.y + z*vec.z;
}

float Vector3::AngleWith(const Vector3 &vec) const {
    return (float)acos( DotProduct(vec))/(GetLength() * vec.GetLength());
}

Vector3 Vector3::CrossProduct(const Vector3 &vec) const {
    Vector3 result;

    result.Set(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	
    return result;
}


Vector3 Vector3::Lerp(const Vector3 &v2, float factor) const {
    return (*this)*(1.0f-factor) + v2*factor;
}

Vector3 Vector3::QuadraticInterpolate(const Vector3 &v2, const Vector3 &v3, float factor) const {
    return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
}


Vector3 Vector3::operator+(const Vector3 &vec) const {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

Vector3 Vector3::operator-(const Vector3 &vec) const {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

Vector3 Vector3::operator*(const float f) const {
    return Vector3(x*f, y*f, z*f);
}

Vector3 Vector3::operator*(const Vector3& vec) const {
    return Vector3(x*vec.x, y*vec.y, z*vec.z);
}

Vector3 Vector3::operator/(const float f) const {
    return (f==0.0f) ? Vector3(0.0f, 0.0f, 0.0f) : Vector3(x / f, y / f, z / f);
}


bool Vector3::operator==(const Vector3 &vec) const {
    if(x==vec.x && y==vec.y && z==vec.z)
        return true;

    return false;
}

bool Vector3::operator!=(const Vector3 &vec) const {
    return !((*this)==vec);
}

Vector3& Vector3::operator+=(const Vector3 &vec) {
    x+=vec.x;
    y+=vec.y;
    z+=vec.z;

    return (*this);
}

Vector3& Vector3::operator-=(const Vector3 &vec) {
    x-=vec.x;
    y-=vec.y;
    z-=vec.z;

    return (*this);
}

Vector3& Vector3::operator*=(const float f) {
    x*=f;
    y*=f;
    z*=f;

    return (*this);
}

Vector3& Vector3::operator*=(const Vector3& vec) {
    x*=vec.x;
    y*=vec.y;
    z*=vec.z;

    return (*this);
}

Vector3& Vector3::operator/=(const float f) {
    if(f!=0.0f) {
        x/=f;
        y/=f;
        z/=f;
    }
    return (*this);
}


Vector3 Vector3::GetRotatedX(double angle) const {
    if(angle==0.0)
        return (*this);

    float sinAngle=(float)sin(M_PI*angle/180);
    float cosAngle=(float)cos(M_PI*angle/180);

    return Vector3(x, y*cosAngle - z*sinAngle, y*sinAngle + z*cosAngle);
}

void Vector3::RotateX(double angle) {
    (*this)=GetRotatedX(angle);
}

Vector3 Vector3::GetRotatedY(double angle) const {
    if(angle==0.0)
        return (*this);

    float sinAngle=(float)sin(M_PI*angle/180);
    float cosAngle=(float)cos(M_PI*angle/180);

    return Vector3(x*cosAngle + z*sinAngle, y, -x*sinAngle + z*cosAngle);
}

void Vector3::RotateY(double angle) {
    (*this)=GetRotatedY(angle);
}

Vector3 Vector3::GetRotatedZ(double angle) const {
    if(angle==0.0)
        return (*this);

    float sinAngle=(float)sin(M_PI*angle/180);
    float cosAngle=(float)cos(M_PI*angle/180);

    return Vector3(x*cosAngle - y*sinAngle, x*sinAngle + y*cosAngle, z);
}

void Vector3::RotateZ(double angle)
{
    (*this)=GetRotatedZ(angle);
}

Vector3 Vector3::GetRotatedAxis(double angle, const Vector3 & axis) const {
    if(angle==0.0)
        return (*this);

    Vector3 u=axis.GetNormalized();

    Vector3 rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

    float sinAngle=(float)sin(M_PI*angle/180);
    float cosAngle=(float)cos(M_PI*angle/180);
    float oneMinusCosAngle=1.0f-cosAngle;

    rotMatrixRow0.x=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
    rotMatrixRow0.y=(u.x)*(u.y)*(oneMinusCosAngle) - sinAngle*u.z;
    rotMatrixRow0.z=(u.x)*(u.z)*(oneMinusCosAngle) + sinAngle*u.y;

    rotMatrixRow1.x=(u.x)*(u.y)*(oneMinusCosAngle) + sinAngle*u.z;
    rotMatrixRow1.y=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
    rotMatrixRow1.z=(u.y)*(u.z)*(oneMinusCosAngle) - sinAngle*u.x;

    rotMatrixRow2.x=(u.x)*(u.z)*(oneMinusCosAngle) - sinAngle*u.y;
    rotMatrixRow2.y=(u.y)*(u.z)*(oneMinusCosAngle) + sinAngle*u.x;
    rotMatrixRow2.z=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));

    return Vector3(DotProduct(rotMatrixRow0),
                   DotProduct(rotMatrixRow1),
                   DotProduct(rotMatrixRow2));
}

void Vector3::RotateAxis(double angle, const Vector3 & axis) {
    (*this)=GetRotatedAxis(angle, axis);
}


void Vector3::PackTo01() {
    (*this)=GetPackedTo01();
}

Vector3 Vector3::GetPackedTo01() const {
    Vector3 temp(*this);

    temp.Normalize();

    temp=temp*0.5f+Vector3(0.5f, 0.5f, 0.5f);

    return temp;
}

Vector3 Vector3::Reflect(const Vector3& normal) const {
    Vector3 reflected = (*this) - (2 * DotProduct(normal)*normal);
    return reflected;
}

Vector3 Vector3::Refract(const Vector3 &normal, float eta) const {
    Vector3 refracted;

    float dot = -DotProduct(normal);
    float k = 1.0f - eta*eta*(1.0f-dot*dot);

    if(k>0)
        refracted = eta*(*this)+(eta*dot-sqrtf(k))*normal;

    return refracted;
}

Vector3 operator*(float scaleFactor, const Vector3 &vec) {
    return vec*scaleFactor;
}

QDebug operator<<(QDebug dbg, const Vector3 &v) {
    dbg.nospace() << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return dbg.space();
}

