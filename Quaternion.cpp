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

float Quaternion::length() const {
    return sqrt(w*w + x*x + y*y + z*z);
}

void Quaternion::normalize() {
    float len = length();
    if (len > 0.0f) {
        w /= len; x /= len; y /= len; z /= len;
    }
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

void Quaternion::test() {
    std::ofstream output("QuaternionResults.txt");
    if (!output.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
    }

    // --- Test konstruktora i podstawowych operacji ---
    Quaternion q1(1, 2, 3, 4);
    Quaternion q2(0.5, -1, 0, 2);

    output << "q1 = (" << q1.w << ", " << q1.x << ", " << q1.y << ", " << q1.z << ")\n";
    output << "q2 = (" << q2.w << ", " << q2.x << ", " << q2.y << ", " << q2.z << ")\n";

    Quaternion qAdd = q1 + q2;
    output << "q1 + q2 = (" << qAdd.w << ", " << qAdd.x << ", " << qAdd.y << ", " << qAdd.z << ")\n";

    Quaternion qSub = q1 - q2;
    output << "q1 - q2 = (" << qSub.w << ", " << qSub.x << ", " << qSub.y << ", " << qSub.z << ")\n";

    Quaternion qMul = q1 * q2;
    output << "q1 * q2 = (" << qMul.w << ", " << qMul.x << ", " << qMul.y << ", " << qMul.z << ")\n";

    Quaternion qDiv = q1 / q2;
    output << "q1 / q2 = (" << qDiv.w << ", " << qDiv.x << ", " << qDiv.y << ", " << qDiv.z << ")\n";

    output << "length(q1) = " << q1.length() << "\n";
    output << "Inverse(q1) = (" << q1.getInverse().w << ", " << q1.getInverse().x << ", "
           << q1.getInverse().y << ", " << q1.getInverse().z << ")\n";

    // --- Test obrotu punktu [-1,-1,-1] o 270° wokół osi X ---
    Vector point(-1, -1, -1);
    Quaternion rot = Quaternion::fromAxisAngle(270, 1, 0, 0); // 270° wokół osi X
    Vector rotated = rot.rotateVector(point);
    output << "\nPunkt [-1,-1,-1] po obrocie 270° wokół osi X: ("
           << rotated.x << ", " << rotated.y << ", " << rotated.z << ")\n";

    // --- Test nieprzemienności mnożenia kwaternionów ---
    Quaternion qA = Quaternion::fromAxisAngle(90, 1, 0, 0);
    Quaternion qB = Quaternion::fromAxisAngle(90, 0, 1, 0);
    Quaternion qAB = qA * qB;
    Quaternion qBA = qB * qA;

    output << "\nqA * qB = (" << qAB.w << ", " << qAB.x << ", " << qAB.y << ", " << qAB.z << ")\n";
    output << "qB * qA = (" << qBA.w << ", " << qBA.x << ", " << qBA.y << ", " << qBA.z << ")\n";
    output << "Widać, że qA*qB != qB*qA (brak przemienności)\n";

    output.close();

    std::cout << "Testy zapisane w pliku " << "QuaternionResults.txt" << std::endl;
}