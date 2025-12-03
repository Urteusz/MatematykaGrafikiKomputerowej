#include <iostream>
#include <fstream>
#include "Vector.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "mathFunctions.h"

using namespace std;

int main()
{
    Vector punktPrzeciecia;
    Prosta prostaA;
    Prosta prostaB;

    prostaA.punkt = {-2,4,0};
    prostaA.kierunek = {3,1,5};

    prostaB.punkt = {-2,4,0};
    prostaB.kierunek = {1,-5,3};

    if (znajdzPrzeciecieProstych( prostaA,  prostaB, punktPrzeciecia))
    {
        printVector("1. Punkt przeciecia", punktPrzeciecia);
    }
    else
    {
        cout << "Proste sa rownolegle, brak punktu przeciecia." << endl;
    }

    float kat = znajdzKatMiedzyProstymi(prostaA, prostaB);
    cout << "2. Kat miedzy prostymi wynosi: " << kat << " stopni." << endl;

    Prosta prosta3 = {Vector(-2, 2, -1), Vector(3, -1, 2)};
    Plaszczyzna plaszczyzna3 = {Vector(2, 3, 3), -8};
    Vector p3;
    if (znajdzPrzeciecieProstejZPlaszczyzna(prosta3, plaszczyzna3, p3))
    {
        printVector("3. Punkt przeciecia", p3);
    }
    else
    {
        cout << "3. Prosta jest rownolegla do plaszczyzny." << endl;
    }
    cout << "4. Kat prosta-plaszczyzna: " << znajdzKatMiedzyProstaAPlaszczyzna(prosta3, plaszczyzna3) << " stopni." << endl;

    Plaszczyzna plaszczyzna5A = {Vector(2, -1, 1), -8};
    Plaszczyzna plaszczyzna5B = {Vector(4, 3, 1), 14};
    Prosta prosta5;
    if (znajdzProstaPrzecieciaPlaszczyzn(plaszczyzna5A, plaszczyzna5B, prosta5))
    {
        cout << "5. Prosta przeciecia:" << endl;
        printVector("   Punkt na prostej", prosta5.punkt);
        printVector("   Wektor kierunkowy", prosta5.kierunek);
    }
    else
    {
        cout << "5. Plaszczyzny sa rownolegle." << endl;
    }
    cout << "6. Kat miedzy plaszczyznami: " << znajdzKatMiedzyPlaszczyznami(plaszczyzna5A, plaszczyzna5B) << " stopni." << endl;

    Vector A(5, 5, 4), A_prim(10, 10, 6);
    Vector B(5, 5, 5), B_prim(10, 10, 3);
    Vector p7;
    if (znajdzPrzeciecieOdcinkow(A, A_prim, B, B_prim, p7))
    {
        printVector("7. Odcinki przecinaja sie w punkcie", p7);
    }
    else
    {
        cout << "7. Odcinki nie przecinaja sie." << endl;
    }

    Sfera sfera_z_zadania;
    sfera_z_zadania.srodek = Vector(0, 0, 0);
    sfera_z_zadania.promien = sqrt(26.0f);

    Vector A7(3, -1, -2);
    Vector A7_prim(5, 3, -4);

    Prosta prosta_z_zadania;
    prosta_z_zadania.punkt = A7;
    prosta_z_zadania.kierunek = A7_prim - A7;


    cout << endl << "Zadanie sfera i prosta:" << endl;
    printVector("Prosta przechodzi przez punkt A", prosta_z_zadania.punkt);
    printVector("Wektor kierunkowy prostej (A' - A)", prosta_z_zadania.kierunek);
    cout << "Sfera ma srodek w [0,0,0] i promien " << sfera_z_zadania.promien << endl << endl;

    std::vector<Vector> punkty;
    int liczbaPunktow = znajdzPrzeciecieProstejZeSfera(prosta_z_zadania, sfera_z_zadania, punkty);

    if (liczbaPunktow == 0)
    {
        cout << "Wynik: Prosta nie przecina sfery." << endl;
    }
    else
    {
        cout << "Wynik: Znaleziono " << liczbaPunktow << " punkt(y) przeciecia:" << endl;
        for (int i = 0; i < punkty.size(); ++i)
        {
            string nazwa = "P" + to_string(i + 1);
            printVector(nazwa.c_str(), punkty[i]);
        }
    }
    return 0;
}
