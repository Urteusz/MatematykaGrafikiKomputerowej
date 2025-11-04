//
// Created by igork on 04.11.2025.
//

#include "Matrix4x4.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Konstruktory ---

Matrix4x4::Matrix4x4() {
    LoadIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4 &mat) {
    memcpy(entries, mat.entries, 16 * sizeof(float)); // [cite: 139]
}

Matrix4x4::Matrix4x4(const float* pF) {
    memcpy(entries, pF, 16 * sizeof(float)); // [cite: 141]
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
    // Implementacja prosto z PDF
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

// POPRAWNA implementacja mnożenia macierzy (Column-Major)
Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) { // Iteruj po kolumnach wynikowych
        for (int j = 0; j < 4; ++j) { // Iteruj po wierszach wynikowych
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                // result(j, i) = A(j, k) * B(k, i)
                sum += entries[j + k * 4] * mat.entries[k + i * 4];
            }
            result.entries[j + i * 4] = sum;
        }
    }
    return result;
}

// KLUCZOWA implementacja Mnożenia Macierz * Wektor
Vector Matrix4x4::operator*(const Vector &v) const {
    Vector result;
    // Traktujemy wektor v (x,y,z) jako (x,y,z,1) - czyli punkt w przestrzeni
    float w = 1.0f;

    result.x = entries[0] * v.x + entries[4] * v.y + entries[8]  * v.z + entries[12] * w;
    result.y = entries[1] * v.x + entries[5] * v.y + entries[9]  * v.z + entries[13] * w;
    result.z = entries[2] * v.x + entries[6] * v.y + entries[10] * v.z + entries[14] * w;

    // Obliczamy wynikowy komponent 'w' (ważne przy perspektywie)
    float result_w = entries[3] * v.x + entries[7] * v.y + entries[11] * v.z + entries[15] * w;

    // Dzielenie przez 'w' (perspective divide)
    if (result_w != 0.0f && result_w != 1.0f) {
        result.x /= result_w;
        result.y /= result_w;
        result.z /= result_w;
    }
    return result;
}

// --- Metody transformacji ---

// Implementacja dla Rotacji wokół Y (potrzebna do zadania)
void Matrix4x4::SetRotationY(const double angle) {
    LoadIdentity(); // [cite: 417]
    float rad = M_PI * angle / 180.0; // Kąt na radiany
    entries[0] = (float)cos(rad);
    entries[2] = (float)sin(rad);
    entries[8] = -(float)sin(rad);
    entries[10] = (float)cos(rad);
}

// Implementacje dla X i Z (dla kompletności, skopiowane z PDF)
void Matrix4x4::SetRotationX(const double angle) {
    LoadIdentity(); // [cite: 411]
    float rad = M_PI * angle / 180.0;
    entries[5] = (float)cos(rad);
    entries[6] = (float)sin(rad);
    entries[9] = -entries[6];
    entries[10] = entries[5];
}

void Matrix4x4::SetRotationZ(const double angle) {
    LoadIdentity(); // [cite: 423]
    float rad = M_PI * angle / 180.0;
    entries[0] = (float)cos(rad);
    entries[1] = (float)sin(rad);
    entries[4] = -entries[1];
    entries[5] = entries[0];
}

// Inne transformacje
void Matrix4x4::SetTranslationPart(const Vector &translation) {
    // Ta metoda zakłada, że macierz jest już ustawiona (np. na jednostkową)
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