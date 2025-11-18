

#ifndef QUATERNION_H
#define UATERNION_H
#include "Vector.h"

class Quaternion {
public:
    float w;  // część skalarna
    float x, y, z;  // część wektorowa

    // Konstruktory
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float scalar, const Vector& vec);

    // Operacje
    Quaternion operator+(const Quaternion& q) const;
    Quaternion operator-(const Quaternion& q) const;
    Quaternion operator*(const Quaternion& q) const;

    Quaternion operator/(const Quaternion &q) const;

    Quaternion operator*(float scalar) const;

    float length() const;
    void normalize();
    Quaternion getConjugate() const;
    Quaternion getInverse() const;

    Vector rotateVector(const Vector& v) const;
    static Quaternion fromAxisAngle(float angleDegrees, float axisX, float axisY, float axisZ);
};

#endif QUATERNION_H