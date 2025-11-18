#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w
    );
}

float Quaternion::length() const {
    return sqrt(w*w + x*x + y*y + z*z);
}

void Quaternion::normalize() {
    float len = length();
    if (len > 0.0f) {
        w /= len; x /= len; y /= len; z /= len;
    }
}

Quaternion Quaternion::getConjugate() const {
    return Quaternion(w, -x, -y, -z);
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