#include <iostream>
#include <cmath>
#include <vector>

#include "Vector.h"
#include "Matrix4x4.h"

#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

const float EPSILON = 0.0001f;

struct Prosta {
    Vector punkt; // Dowolny punkt na prostej
    Vector kierunek; // Wektor kierunkowy prostej
};

struct Plaszczyzna {
    Vector normal; // Wektor normalny (A, B, C)
    float d;       // Współczynnik D
};
struct Sfera {
    Vector srodek;
    float promien;
};

float ClearZero(float);
void printVector(const char* name, Vector v);
void printMatrix(const char* name, Matrix4x4 m);
bool znajdzPrzeciecieProstych(Prosta& p1, Prosta& p2, Vector& out);
float znajdzKatMiedzyProstymi(Prosta& p1, Prosta& p2);
bool znajdzPrzeciecieProstejZPlaszczyzna(Prosta& prosta, Plaszczyzna& plaszczyzna, Vector& punktPrzeciecia);
float znajdzKatMiedzyProstaAPlaszczyzna(Prosta& prosta, Plaszczyzna& plaszczyzna);
bool znajdzProstaPrzecieciaPlaszczyzn(Plaszczyzna& p1, Plaszczyzna& p2, Prosta& out);
float znajdzKatMiedzyPlaszczyznami(Plaszczyzna& p1, Plaszczyzna& p2);
bool znajdzPrzeciecieOdcinkow(Vector& a1, Vector& a2, Vector& b1, Vector& b2, Vector& punktPrzeciecia);
int znajdzPrzeciecieProstejZeSfera(Prosta& prosta, Sfera& sfera, std::vector<Vector>& punktyPrzeciecia);

#endif