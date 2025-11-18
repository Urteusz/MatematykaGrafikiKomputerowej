#include <iostream>
#include <fstream>
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

        ofstream output("QuaternionResults.txt");
    if (!output.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
        return 1;
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

    std::cout << "Testy zapisane w pliku QuaternionResults.txt" << std::endl;
    return 0;
}
