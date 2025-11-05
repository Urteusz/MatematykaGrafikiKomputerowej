#include <iostream>
#include <cmath>
#include "Vector.h"
#include "Matrix4x4.h"
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

    Matrix4x4 M_F = M_B * 3.0f;
    printMatrix("Test 3: Mnozenie B * 3.0 (powinny byc same 6)", M_F);

    // Test 4: Mnożenie Macierz * Macierz (M * I)
    Matrix4x4 M_Trans; // Tworzymy macierz translacji
    M_Trans.SetTranslationPart(Vector(5, 6, 7));

    Matrix4x4 M_Result = M_Trans * M_I; // Mnożymy przez jednostkową

    printMatrix("Test 4: Macierz Translacji (M_Trans)", M_Trans);
    printMatrix("Wynik M_Trans * I (powinien byc identyczny)", M_Result);
    cout << endl;


    // --- ZADANIE 3: Obrót wektora ---
    cout << "--- Zadanie 3: Obracanie wektora [1,0,0] o 90 stopni wokol osi Y ---" << endl;

    // 1. Stwórz wektor [1, 0, 0]
    Vector v(1, 0, 0);
    printVector("Wektor poczatkowy", v);

    // 2. Stwórz macierz obrotu o 90 stopni wokół osi Y
    Matrix4x4 rotY;
    rotY.SetRotationY(90.0);
    printMatrix("Macierz obrotu RotY(90)", rotY);

    // 3. Pomnóż macierz przez wektor
    // v_nowy = M * v_stary
    Vector v_rotated = rotY * v;

    // Wynik obrotu [1,0,0] o 90 stopni wokół Y powinien dać [0,0,-1]
    printVector("Wektor po obrocie", v_rotated);
    cout << endl;


    // --- ZADANIE 4: Brak przemienności mnożenia macierzy ---
    cout << "--- Zadanie 4: Dowod na brak przemiennosci mnozenia (A*B != B*A) ---" << endl;

    // Stwórzmy dwie różne macierze:
    // A = Translacja o (10, 0, 0)
    Matrix4x4 A;
    A.SetTranslationPart(Vector(10, 0, 0));
    printMatrix("Macierz A (Translacja o 10 na X)", A);

    // B = Rotacja o 90 stopni wokół Y
    Matrix4x4 B;
    B.SetRotationY(90.0);
    printMatrix("Macierz B (Rotacja o 90 na Y)", B);

    // Oblicz A * B (najpierw obrót, potem translacja)
    Matrix4x4 AB = A * B;
    printMatrix("Wynik A * B", AB);

    // Oblicz B * A (najpierw translacja, potem obrót)
    Matrix4x4 BA = B * A;
    printMatrix("Wynik B * A", BA);

    cout << "Jak widac, macierze A * B i B * A sa rozne, co udowadnia brak przemiennosci." << endl;

    //zapisać wyniki w dokumencie Word.
    return 0;
}
