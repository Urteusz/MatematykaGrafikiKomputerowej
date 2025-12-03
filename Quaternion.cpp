#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator-(const Quaternion& q) const {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w
    );
}

Quaternion Quaternion::operator/(const Quaternion& q) const {
    return (*this) * q.getInverse();
}

Quaternion Quaternion::getInverse() const {
    float lenSq = w*w + x*x + y*y + z*z;
    if (lenSq > 0.0f) {
        float inv = 1.0f / lenSq;
        return Quaternion(w*inv, -x*inv, -y*inv, -z*inv);
    }
    return Quaternion(1, 0, 0, 0);
}

Vector Quaternion::rotateVector(const Vector& v) const {
    Quaternion vecQuat(0, v.x, v.y, v.z);
    Quaternion result = (*this) * vecQuat * this->getInverse();
    return Vector(result.x, result.y, result.z);
}

Quaternion Quaternion::fromAxisAngle(float angleDegrees, float axisX, float axisY, float axisZ) {
    float angleRad = angleDegrees * M_PI / 180.0f;
    float halfAngle = angleRad / 2.0f;

    float sinHalf = sin(halfAngle);
    float cosHalf = cos(halfAngle);

    float length = sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);
    if (length < 0.0001f) return Quaternion(1, 0, 0, 0);

    float k = sinHalf / length;

    return Quaternion(cosHalf, axisX * k, axisY * k, axisZ * k);
}