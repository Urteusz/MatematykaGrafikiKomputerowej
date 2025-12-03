//
// Created by Urteusz on 22.10.2025.
//

#ifndef OPENGLEXAMPLE_VECTOR_H
#define OPENGLEXAMPLE_VECTOR_H

#include <string>
#include <iostream>
#include <cmath>

class Vector {
public:
    float x,y,z;

    Vector();
    Vector(float x, float y, float z);
    Vector(const Vector &v);
    ~Vector();

    void operator+=(const Vector& v);
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(float scalar) const;
    void add(Vector v);
    void sub(Vector v);
    void div(float f);
    void mag(float f);
    float length();
    void normalize();
    float dotProduct(Vector v);
    Vector cross(Vector v);
};


#endif //OPENGLEXAMPLE_VECTOR_H
