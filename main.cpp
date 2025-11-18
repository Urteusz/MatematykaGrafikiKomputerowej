#include <iostream>
#include <cmath>
#include "Vector.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

using namespace std;

const float EPSILON = 0.0001f;

float ClearZero(float val){
    if(fabs(val) < EPSILON){
        val = 0.0f;
    }
    return val;
}

// Mała funkcja pomocnicza do ładnego drukowania wektorów
void printVector(const char* name, Vector v) {
    cout << name << " = [" << ClearZero(v.x) << ", " << ClearZero(v.y) << ", " << ClearZero(v.z) << "]" << endl;
}

// Mała funkcja pomocnicza do ładnego drukowania macierzy (kolumnowo)
void printMatrix(const char* name, Matrix4x4 m) {
    cout << "--- " << name << " ---" << endl;
    for(int row = 0; row < 4; ++row) {
        cout << "| " << ClearZero(m.entries[row + 0]) << "\t"
             << ClearZero(m.entries[row + 4]) << "\t"
             << ClearZero(m.entries[row + 8]) << "\t"
             << ClearZero(m.entries[row + 12]) << " |" << endl;
    }
    cout << "--------------" << endl;
}

Quaternion createRotationX(float angleDegrees) {
    float angleRad = angleDegrees * M_PI / 180.0f;
    float halfAngle = angleRad / 2.0f;

    return Quaternion(
        cos(halfAngle),    // w
        -sin(halfAngle),    // x (oś X)
        0,                 // y
        0                  // z
    );
}

Quaternion createRotationY(float angleDegrees) {
    float angleRad = angleDegrees * M_PI / 180.0f;
    float halfAngle = angleRad / 2.0f;

    return Quaternion(
        cos(halfAngle),    // w
        0,                 // x
        sin(halfAngle),    // y (oś Y)
        0                  // z
    );
}

Quaternion createRotationZ(float angleDegrees) {
    float angleRad = angleDegrees * M_PI / 180.0f;
    float halfAngle = angleRad / 2.0f;

    return Quaternion(
        cos(halfAngle),    // w
        0,                 // x
        0,                 // y
        sin(halfAngle)     // z (oś Z)
    );
}

int main() {

    // Test 1: LoadIdentity()
    Matrix4x4 M_I;
    M_I.LoadIdentity();
    printMatrix("Test 1: Macierz Jednostkowa (I)", M_I);

    // Test 2: Dodawanie (A + B)
    Matrix4x4 M_A(1.0f, 1.0f, 1.0f, 1.0f,
                  1.0f, 1.0f, 1.0f, 1.0f,
                  1.0f, 1.0f, 1.0f, 1.0f,
                  1.0f, 1.0f, 1.0f, 1.0f);

    Matrix4x4 M_B(2.0f, 2.0f, 2.0f, 2.0f,
                  2.0f, 2.0f, 2.0f, 2.0f,
                  2.0f, 2.0f, 2.0f, 2.0f,
                  2.0f, 2.0f, 2.0f, 2.0f);

    Matrix4x4 M_C = M_A + M_B;
    Matrix4x4 M_D = M_B + M_A;


    cout << "Test dodawania:" << endl;
    printMatrix("Dodawanie M_A+M_B", M_C);
    printMatrix("Dodawanie M_B+M_a", M_D);

    if(M_C == M_D)
    {
        cout << "Implementacja dodowania jest przemienne" << endl;
    }
    else
    {
        cout << "Implementacja dodawnaia nie jest przemienna" << endl;
    }

    cout << endl;

    // Mnożenie przez skalar
    Matrix4x4 M_F = M_B * 3.0f;
    printMatrix("Test 3: Mnozenie B * 3.0 (powinny byc same 6)", M_F);

    cout << endl;

    cout << "--- Test transpozycji macierzy ---" << endl;
    Matrix4x4 M_T(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16);

    // Transpozycja test
    printMatrix("Macierz oryginalna", M_T);

    Matrix4x4 M_Tt = M_T.GetTransposed();
    printMatrix("Macierz transponowana", M_Tt);

    Matrix4x4 M_Ttt;
    M_Ttt.SetTransposed(M_Tt);
    printMatrix("Ponownie transponowana (powinna byc jak oryginalna)", M_Ttt);

    if (M_T == M_Ttt)
        cout << "Transpozycja dziala poprawnie (T(T(M)) == M)" << endl;
    else
        cout << "Blad transpozycji (T(T(M)) != M)" << endl;

    cout << endl;

    // Macierz odwrotna
    cout << "--- Test macierzy odwrotnej ---" << endl;

    Matrix4x4 M_inv_test(
            1, 2, 3, 0,
            0, 1, 4, 0,
            5, 6, 0, 0,
            0, 0, 0, 1);

    printMatrix("Macierz M", M_inv_test);

    Matrix4x4 M_inv = M_inv_test.getInverseOfMatrix();
    printMatrix("Macierz odwrotna M^-1", M_inv);

    Matrix4x4 M_prod = M_inv_test * M_inv;
    printMatrix("M * M^-1 (powinna byc jednostkowa)", M_prod);

    Matrix4x4 I;
    I.LoadIdentity();

    if (M_prod == I)
        cout << "Macierz odwrotna dziala poprawnie (M * M^-1 == I)" << endl;
    else
        cout << "Blad macierzy odwrotnej (M * M^-1 != I)" << endl;

    cout << endl;

    // --- ZADANIE 3: Obrót wektora ---
    cout << "--- Zadanie 3: Obracanie wektora [1,0,0] o 90 stopni wokol osi Y ---" << endl;

    Vector v(1, 0, 0);
    printVector("Wektor poczatkowy", v);

    Matrix4x4 rotY;
    rotY.SetRotationY(90.0);
    printMatrix("Macierz obrotu RotY(90)", rotY);

    Vector v_rotated = rotY * v;

    printVector("Wektor po obrocie", v_rotated);
    cout << endl;


    // --- ZADANIE 4: Brak przemienności mnożenia macierzy ---

    cout << "--- Zadanie 4: Przemiennosc mnozenia prostych macierzy ---" << endl;

    Matrix4x4 M1(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16);

    Matrix4x4 M2(
            2, 0, 1, 3,
            1, 2, 0, 1,
            3, 1, 2, 0,
            0, 3, 1, 2);

    Matrix4x4 M1M2 = M1 * M2;
    Matrix4x4 M2M1 = M2 * M1;

    printMatrix("M1 * M2", M1M2);
    printMatrix("M2 * M1", M2M1);

    if (M1M2 == M2M1)
        cout << "Mnozenie macierzy jest przemienne (blad)" << endl;
    else
        cout << "Mnozenie macierzy NIE jest przemienne (poprawnie)" << endl;

    cout << endl;

    cout << endl << "=== TESTY KWATERNIONOW ===" << endl << endl;

    // 1. Przygotowanie prostych obrotów
    Quaternion qx_45 = createRotationX(45.0f);
    Quaternion qy_45 = createRotationY(45.0f);

    // 2. Mnozenie w dwóch kolejnosciach
    Quaternion qxqy = qx_45 * qy_45;
    Quaternion qyqx = qy_45 * qx_45;

    cout << "--- Mnozenie kwaternionow ---" << endl;
    cout << "qX * qY = [" << qxqy.w << ", " << qxqy.x << ", " << qxqy.y << ", " << qxqy.z << "]" << endl;
    cout << "qY * qX = [" << qyqx.w << ", " << qyqx.x << ", " << qyqx.y << ", " << qyqx.z << "]" << endl;

    cout << endl << "Wniosek: qX*qY i qY*qX sa rozne → mnozenie NIE jest przemienne." << endl << endl;



    // 3. Sprawdzenie różnicy na przykładzie rotacji punktu
    Vector testPoint(1, 0, 0);
    Vector rot1 = qxqy.rotateVector(testPoint);
    Vector rot2 = qyqx.rotateVector(testPoint);

    cout << "--- Rotacja punktu (1,0,0) dwiema kolejnosciami obrotow ---" << endl;
    printVector("Rotacja qX*qY", rot1);
    printVector("Rotacja qY*qX", rot2);

    if (rot1.x != rot2.x || rot1.y != rot2.y || rot1.z != rot2.z)
        cout << "Potwierdzenie: wyniki różne → brak przemiennosci." << endl;
    else
        cout << "Blad: wyniki identyczne (nie powinny byc)." << endl;

    cout << endl;



    // 4. Obrót punktu [-1,-1,-1] o 270° wokół osi X
    cout << "--- Obrót punktu [-1,-1,-1] o 270° wokol osi X ---" << endl;

    Quaternion qx_270 = createRotationX(270.0f);
    qx_270.normalize();

    Vector p(-1, -1, -1);
    printVector("Punkt przed obrotem", p);

    Vector p_rot = qx_270.rotateVector(p);
    printVector("Punkt po obrocie", p_rot);

    cout << "Oczekiwany wynik: [-1, 1, -1]" << endl;

    cout << endl << "=== KONIEC TESTOW ===" << endl;
    return 0;
}
