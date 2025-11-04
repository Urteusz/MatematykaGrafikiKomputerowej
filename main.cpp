#include <iostream>
#include <cmath>
#include "Vector.h"
using namespace std;

int main() {

    // 2. Sprawdzenie przemienności dodawania
    Vector a(1, 2, 3);
    Vector b(4, 5, 6);
    Vector sum1 = a;
    sum1.add(b);
    Vector sum2 = b;
    sum2.add(a);

    cout << "Przemiennosc dodawania: "
         << ((sum1.x == sum2.x && sum1.y == sum2.y && sum1.z == sum2.z) ? "TAK" : "NIE")
         << endl;

    // 3. Kąt między [0,3,0] a [5,5,0]
    Vector v1(0, 3, 0);
    Vector v2(5, 5, 0);
    float dot = v1.dotProduct(v2);
    float len1 = v1.length();
    float len2 = v2.length();
    float angle = acos(dot / (len1 * len2)); // w radianach
    cout << "Kat miedzy wektorami: " << angle * 180 / M_PI << " stopni" << endl;

    // 4. Wektor prostopadły do [4,5,1] i [4,1,3]
    Vector v3(4, 5, 1);
    Vector v4(4, 1, 3);
    Vector cross = v3.cross(v4);
    cout << "Wektor prostopadly: [" << cross.x << ", " << cross.y << ", " << cross.z << "]" << endl;

    // 5. Normalizacja tego wektora
    cross.normalize();
    cout << "Znormalizowany wektor: [" << cross.x << ", " << cross.y << ", " << cross.z << "]" << endl;
    cout << "Wektor zostalW "
         << (cross.length() == 1.f ? "eobrze znormalizowany" : "zle znormalizowany")
         << endl;

    return 0;
}
