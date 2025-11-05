#include "Matrix4x4.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Konstruktory ---

Matrix4x4::Matrix4x4() {
    LoadIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4 &mat) {
    memcpy(entries, mat.entries, 16 * sizeof(float));
}

Matrix4x4::Matrix4x4(const float* pF) {
    memcpy(entries, pF, 16 * sizeof(float));
}

Matrix4x4::Matrix4x4(float e0, float e1, float e2, float e3,
                     float e4, float e5, float e6, float e7,
                     float e8, float e9, float e10, float e11,
                     float e12, float e13, float e14, float e15) {
    // Wypełnianie kolumnami
    entries[0] = e0; entries[1] = e1; entries[2] = e2; entries[3] = e3;
    entries[4] = e4; entries[5] = e5; entries[6] = e6; entries[7] = e7;
    entries[8] = e8; entries[9] = e9; entries[10] = e10; entries[11] = e11;
    entries[12] = e12; entries[13] = e13; entries[14] = e14; entries[15] = e15;
}

Matrix4x4::~Matrix4x4() {}

// --- Metody pomocnicze ---

void Matrix4x4::LoadIdentity() {
    memset(entries, 0, 16 * sizeof(float));
    entries[0] = 1.0f;
    entries[5] = 1.0f;
    entries[10] = 1.0f;
    entries[15] = 1.0f;
}

// --- Operatory arytmetyczne ---

bool Matrix4x4::operator==(const Matrix4x4& other) const {
    for (int i = 0; i < 16; ++i)
        if (entries[i] != other.entries[i])
            return false;
    return true;
}

bool Matrix4x4::operator!=(const Matrix4x4& other) const {
    return !(*this == other);
}


Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &mat) const {
    return Matrix4x4(entries[0] + mat.entries[0],
                     entries[1] + mat.entries[1],
                    entries[2] + mat.entries[2],
                    entries[3] + mat.entries[3],
                    entries[4] + mat.entries[4],
                    entries[5] + mat.entries[5],
                    entries[6] + mat.entries[6],
                    entries[7] + mat.entries[7],
                    entries[8] + mat.entries[8],
                    entries[9] + mat.entries[9],
                    entries[10] + mat.entries[10],
                    entries[11] + mat.entries[11],
                    entries[12] + mat.entries[12],
                    entries[13] + mat.entries[13],
                    entries[14] + mat.entries[14],
                     entries[15] + mat.entries[15]);
}

Matrix4x4 Matrix4x4::operator*(const float f) const {
    return Matrix4x4(entries[0] * f,
                     entries[1] * f,
                    entries[2] * f,
                    entries[3] * f,
                    entries[4] * f,
                    entries[5] * f,
                    entries[6] * f,
                    entries[7] * f,
                    entries[8] * f,
                    entries[9] * f,
                    entries[10] * f,
                    entries[11] * f,
                    entries[12] * f,
                    entries[13] * f,
                    entries[14] * f,
                     entries[15] * f);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += entries[j + k * 4] * mat.entries[k + i * 4];
            }
            result.entries[j + i * 4] = sum;
        }
    }
    return result;
}

Vector Matrix4x4::operator*(const Vector &v) const {
    Vector result;
    float w = 1.0f;

    result.x = entries[0] * v.x + entries[4] * v.y + entries[8]  * v.z + entries[12] * w;
    result.y = entries[1] * v.x + entries[5] * v.y + entries[9]  * v.z + entries[13] * w;
    result.z = entries[2] * v.x + entries[6] * v.y + entries[10] * v.z + entries[14] * w;

    float result_w = entries[3] * v.x + entries[7] * v.y + entries[11] * v.z + entries[15] * w;

    if (result_w != 0.0f && result_w != 1.0f) {
        result.x /= result_w;
        result.y /= result_w;
        result.z /= result_w;
    }
    return result;
}

// --- Metody transformacji ---

// Implementacja dla rotacji wokół Y
void Matrix4x4::SetRotationY(const double angle) {
    LoadIdentity();
    float rad = M_PI * angle / 180.0;
    entries[0] = (float)cos(rad);
    entries[2] = (float)sin(rad);
    entries[8] = -(float)sin(rad);
    entries[10] = (float)cos(rad);
}

// Implementacje dla rotacji wokół X
void Matrix4x4::SetRotationX(const double angle) {
    LoadIdentity(); // [cite: 411]
    float rad = M_PI * angle / 180.0;
    entries[5] = (float)cos(rad);
    entries[6] = (float)sin(rad);
    entries[9] = -entries[6];
    entries[10] = entries[5];
}

// Implementacja dla rotacji wokół Z
void Matrix4x4::SetRotationZ(const double angle) {
    LoadIdentity();
    float rad = M_PI * angle / 180.0;
    entries[0] = (float)cos(rad);
    entries[1] = (float)sin(rad);
    entries[4] = -entries[1];
    entries[5] = entries[0];
}

// Inne transformacje
void Matrix4x4::SetTranslationPart(const Vector &translation) {
    entries[12] = translation.x;
    entries[13] = translation.y;
    entries[14] = translation.z;
}

void Matrix4x4::SetScale(const Vector &scaleFactor) {
    LoadIdentity();
    entries[0] = scaleFactor.x;
    entries[5] = scaleFactor.y;
    entries[10] = scaleFactor.z;
}
void Matrix4x4::SetTransposed(const Matrix4x4 &m) {
   

    entries[0] = m.entries[0];
    entries[5] = m.entries[5];
    entries[10] = m.entries[10];
    entries[15] = m.entries[15];

    entries[1] = m.entries[4];
    entries[4] = m.entries[1];

    entries[2] = m.entries[8];
    entries[8] = m.entries[2];

    entries[3] = m.entries[12];
    entries[12] = m.entries[3];

    entries[6] = m.entries[9];
    entries[9] = m.entries[6];

    entries[7] = m.entries[13];
    entries[13] = m.entries[7];

    entries[11] = m.entries[14];
    entries[14] = m.entries[11];
}

Matrix4x4 Matrix4x4::GetTransposed() const {
    Matrix4x4 result;
    result.SetTransposed(*this);
    return result;
}

void Matrix4x4::setMatrixAsInverseOfGivenMatrix(const Matrix4x4& m) {
    const float* a = m.entries;
    float inv[16];

    inv[0] = a[5]  * a[10] * a[15] -
             a[5]  * a[11] * a[14] -
             a[9]  * a[6]  * a[15] +
             a[9]  * a[7]  * a[14] +
             a[13] * a[6]  * a[11] -
             a[13] * a[7]  * a[10];

    inv[4] = -a[4]  * a[10] * a[15] +
             a[4]  * a[11] * a[14] +
             a[8]  * a[6]  * a[15] -
             a[8]  * a[7]  * a[14] -
             a[12] * a[6]  * a[11] +
             a[12] * a[7]  * a[10];

    inv[8] = a[4]  * a[9] * a[15] -
             a[4]  * a[11] * a[13] -
             a[8]  * a[5] * a[15] +
             a[8]  * a[7] * a[13] +
             a[12] * a[5] * a[11] -
             a[12] * a[7] * a[9];

    inv[12] = -a[4]  * a[9] * a[14] +
              a[4]  * a[10] * a[13] +
              a[8]  * a[5] * a[14] -
              a[8]  * a[6] * a[13] -
              a[12] * a[5] * a[10] +
              a[12] * a[6] * a[9];

    inv[1] = -a[1]  * a[10] * a[15] +
             a[1]  * a[11] * a[14] +
             a[9]  * a[2] * a[15] -
             a[9]  * a[3] * a[14] -
             a[13] * a[2] * a[11] +
             a[13] * a[3] * a[10];

    inv[5] = a[0]  * a[10] * a[15] -
             a[0]  * a[11] * a[14] -
             a[8]  * a[2] * a[15] +
             a[8]  * a[3] * a[14] +
             a[12] * a[2] * a[11] -
             a[12] * a[3] * a[10];

    inv[9] = -a[0]  * a[9] * a[15] +
             a[0]  * a[11] * a[13] +
             a[8]  * a[1] * a[15] -
             a[8]  * a[3] * a[13] -
             a[12] * a[1] * a[11] +
             a[12] * a[3] * a[9];

    inv[13] = a[0]  * a[9] * a[14] -
              a[0]  * a[10] * a[13] -
              a[8]  * a[1] * a[14] +
              a[8]  * a[2] * a[13] +
              a[12] * a[1] * a[10] -
              a[12] * a[2] * a[9];

    inv[2] = a[1]  * a[6] * a[15] -
             a[1]  * a[7] * a[14] -
             a[5]  * a[2] * a[15] +
             a[5]  * a[3] * a[14] +
             a[13] * a[2] * a[7] -
             a[13] * a[3] * a[6];

    inv[6] = -a[0]  * a[6] * a[15] +
             a[0]  * a[7] * a[14] +
             a[4]  * a[2] * a[15] -
             a[4]  * a[3] * a[14] -
             a[12] * a[2] * a[7] +
             a[12] * a[3] * a[6];

    inv[10] = a[0]  * a[5] * a[15] -
              a[0]  * a[7] * a[13] -
              a[4]  * a[1] * a[15] +
              a[4]  * a[3] * a[13] +
              a[12] * a[1] * a[7] -
              a[12] * a[3] * a[5];

    inv[14] = -a[0]  * a[5] * a[14] +
              a[0]  * a[6] * a[13] +
              a[4]  * a[1] * a[14] -
              a[4]  * a[2] * a[13] -
              a[12] * a[1] * a[6] +
              a[12] * a[2] * a[5];

    inv[3] = -a[1] * a[6] * a[11] +
             a[1] * a[7] * a[10] +
             a[5] * a[2] * a[11] -
             a[5] * a[3] * a[10] -
             a[9] * a[2] * a[7] +
             a[9] * a[3] * a[6];

    inv[7] = a[0] * a[6] * a[11] -
             a[0] * a[7] * a[10] -
             a[4] * a[2] * a[11] +
             a[4] * a[3] * a[10] +
             a[8] * a[2] * a[7] -
             a[8] * a[3] * a[6];

    inv[11] = -a[0] * a[5] * a[11] +
              a[0] * a[7] * a[9] +
              a[4] * a[1] * a[11] -
              a[4] * a[3] * a[9] -
              a[8] * a[1] * a[7] +
              a[8] * a[3] * a[5];

    inv[15] = a[0] * a[5] * a[10] -
              a[0] * a[6] * a[9] -
              a[4] * a[1] * a[10] +
              a[4] * a[2] * a[9] +
              a[8] * a[1] * a[6] -
              a[8] * a[2] * a[5];

    float det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];
    if (fabs(det) < 1e-6f) return;

    det = 1.0f / det;
    for (int i = 0; i < 16; i++)
        entries[i] = inv[i] * det;
}


Matrix4x4 Matrix4x4::getInverseOfMatrix() const {
    Matrix4x4 result;
    result.setMatrixAsInverseOfGivenMatrix(*this);
    return result;
}