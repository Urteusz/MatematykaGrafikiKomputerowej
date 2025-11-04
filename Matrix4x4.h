//
// Created by igork on 04.11.2025.
//

#ifndef OPENGLEXAMPLE_MATRIX4X4_H
#define OPENGLEXAMPLE_MATRIX4X4_H

#include "Vector.h" // Potrzebne do transformacji i mnożenia
#include <cstring>  // Dla memset i memcpy
#include <cmath>    // Dla sin i cos

class Matrix4x4 {
public:
    // Macierz przechowuje 16 liczb float
    // Użyjemy układu kolumnowego (Column-Major), standardowego w OpenGL
    // [ 0  4  8 12 ]
    // [ 1  5  9 13 ]
    // [ 2  6 10 14 ]
    // [ 3  7 11 15 ]
    float entries[16];

    // --- Konstruktory ---
    Matrix4x4(); // Domyślny (stworzy macierz jednostkową)
    Matrix4x4(float e0, float e1, float e2, float e3,
              float e4, float e5, float e6, float e7,
              float e8, float e9, float e10, float e11,
              float e12, float e13, float e14, float e15);
    Matrix4x4(const float* f);
    Matrix4x4(const Matrix4x4& mat);
    ~Matrix4x4();

    // --- Metody pomocnicze ---
    void LoadIdentity();

    // --- Operatory arytmetyczne ---
    Matrix4x4 operator+(const Matrix4x4& mat) const;
    Matrix4x4 operator*(const float f) const;
    Matrix4x4 operator*(const Matrix4x4& mat) const;

    // Mnożenie Macierz * Wektor
    // Traktuje Vector (x,y,z) jako [x, y, z, 1]
    Vector operator*(const Vector& v) const;

    // --- Metody transformacji ---
    void SetTranslationPart(const Vector& translation); //
    void SetScale(const Vector& scaleFactor); //

    // Metody rotacji
    void SetRotationX(const double angle);
    void SetRotationY(const double angle);
    void SetRotationZ(const double angle);

};


#endif //OPENGLEXAMPLE_MATRIX4X4_H
