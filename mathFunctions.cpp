#include "mathFunctions.h"

using namespace std;

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

bool znajdzPrzeciecieProstych(Prosta& prostaA, Prosta& prostaB, Vector& punktPrzeciecia) {

    // Sprawdzenie, czy proste są równoległe.
    Vector cross = prostaA.kierunek.cross(prostaB.kierunek);
    if (cross.length() < EPSILON) {
        return false;
    }

    // Rozwiązujemy układ równań dla parametrów t i s:
    // P_A + t * V_A = P_B + s * V_B
    // t * V_A - s * V_B = P_B - P_A
    Vector dp = prostaB.punkt - prostaA.punkt;

    // Używamy dwóch pierwszych równań (dla x i y) do znalezienia t i s.
    // t*vA.x - s*vB.x = dp.x
    // t*vA.y - s*vB.y = dp.y
    float det = prostaB.kierunek.x * prostaA.kierunek.y - prostaB.kierunek.y * prostaA.kierunek.x;

    // Jeśli wyznacznik jest bliski zeru, oznacza to, że projekcje wektorów na płaszczyznę XY są równoległe.
    if (abs(det) < EPSILON) {
        return false;
    }

    float t = (prostaB.kierunek.x * dp.y - prostaB.kierunek.y * dp.x) / det;
    
    // Najpierw obliczamy punkt na prostej A
    Vector p1 = prostaA.punkt + prostaA.kierunek * t;

    // Sprawdzamy, czy ten punkt leży również na prostej B.
    Vector diff = p1 - prostaB.punkt;

    // Sprawdzamy spójność dla każdej współrzędnej
    float s = 0;
    bool spojnosc = true;
    if (abs(prostaB.kierunek.x) > EPSILON) {
        s = diff.x / prostaB.kierunek.x;
        if (abs(diff.y - s * prostaB.kierunek.y) > EPSILON || abs(diff.z - s * prostaB.kierunek.z) > EPSILON) {
            spojnosc = false;
        }
    }
    else if (abs(prostaB.kierunek.y) > EPSILON) {
        s = diff.y / prostaB.kierunek.y;
        if (abs(diff.x - s * prostaB.kierunek.x) > EPSILON || abs(diff.z - s * prostaB.kierunek.z) > EPSILON) {
            spojnosc = false;
        }
    }
    else {
        s = diff.z / prostaB.kierunek.z;
        if (abs(diff.x - s * prostaB.kierunek.x) > EPSILON || abs(diff.y - s * prostaB.kierunek.y) > EPSILON) {
            spojnosc = false;
        }
    }

    if (spojnosc) {
        punktPrzeciecia = p1;
        return true;
    }

    // Jeśli nie ma spójności, proste są skośne.
    return false;
}

float znajdzKatMiedzyProstymi(Prosta& prostaA, Prosta& prostaB) {
    Vector vA = prostaA.kierunek;
    Vector vB = prostaB.kierunek;

    // Oblicz długości obu wektorów.
    float dlugosc_vA = vA.length();
    float dlugosc_vB = vB.length();

    // Jeśli któryś wektor ma długość 0, kąt jest niezdefiniowany. Zwracamy 0, aby uniknąć dzielenia przez zero.
    if (dlugosc_vA < EPSILON || dlugosc_vB < EPSILON) {
        return 0.0f;
    }

    // Obliczamy iloczyn skalarny wektorów kierunkowych.
    float iloczynSkalarny = vA.dotProduct(vB);

    // Oblicz cosinus kąta. Używamy abs, bo jest to kąt ostry (0-90°) z definicji.
    float cosinusKata = abs(iloczynSkalarny) / (dlugosc_vA * dlugosc_vB);

    // Oblicz kąt w radianach używając arcus cosinus.
    float katRad = acos(cosinusKata);
    float katStopnie = katRad * 180.0f / M_PI;

    return katStopnie;
}

bool znajdzPrzeciecieProstejZPlaszczyzna(Prosta& prosta, Plaszczyzna& plaszczyzna, Vector& punktPrzeciecia) {

    // Iloczyn skalarny wektora kierunkowego prostej (V) i wektora normalnego płaszczyzny (N)
    float dotNV = plaszczyzna.normal.dotProduct(prosta.kierunek);

    // Jeśli iloczyn skalarny jest bliski zeru, to prosta jest równoległa do płaszczyzny.
    if (abs(dotNV) < EPSILON) {
        return false;
    }

    // Obliczamy parametr 't' ze wzoru: t = -(N • P₀ + D) / (N • V)
    // gdzie P₀ to punkt na prostej.
    float dotNP0 = plaszczyzna.normal.dotProduct(prosta.punkt);
    float t = -(dotNP0 + plaszczyzna.d) / dotNV;

    // Obliczamy współrzędne punktu przecięcia wstawiając 't' do równania prostej
    punktPrzeciecia = prosta.punkt + prosta.kierunek * t;
    return true;
}

float znajdzKatMiedzyProstaAPlaszczyzna(Prosta& prosta, Plaszczyzna& plaszczyzna) {
    Vector v = prosta.kierunek;
    Vector n = plaszczyzna.normal;

    float dotNV = v.dotProduct(n);
    float lenV = v.length();
    float lenN = n.length();

    if (lenV < EPSILON || lenN < EPSILON) return 0.0f;

    // sin(alfa) = |N • V| / (||N|| * ||V||)
    float sinAlfa = abs(dotNV) / (lenV * lenN);

    // Kąt w radianach to arcus sinus
    float katRad = asin(sinAlfa);
    return katRad * 180.0f / M_PI;
}

bool znajdzProstaPrzecieciaPlaszczyzn(Plaszczyzna& pA, Plaszczyzna& pB, Prosta& prostaWynikowa) {

    // Oblicz wektor kierunkowy prostej jako iloczyn wektorowy normalnych
    Vector kierunek = pA.normal.cross(pB.normal);

    // Jeśli długość wektora kierunkowego jest bliska 0, normalne i płaszczyzny są równoległe, czyli nie ma prostej przecięcia.
    if (kierunek.length() < EPSILON) {
        return false;
    }
    prostaWynikowa.kierunek = kierunek;

    // Znajdź dowolny punkt na prostej przecięcia.
    // Rozwiązujemy układ równań:
    // A1*x + B1*y + C1*z + D1 = 0
    // A2*x + B2*y + C2*z + D2 = 0
    // Zakładamy z=0 i rozwiązujemy dla x i y.
    float det = pA.normal.x * pB.normal.y - pB.normal.x * pA.normal.y;

    if (abs(det) < EPSILON) {
        // To założenie nie zadziałało (prosta jest równoległa do płaszczyzny XY).
        return false;
    }

    // Wzory Cramera dla układu Ax+By=-D
    float x = (pB.normal.y * -pA.d - pA.normal.y * -pB.d) / det;
    float y = (pA.normal.x * -pB.d - pB.normal.x * -pA.d) / det;

    prostaWynikowa.punkt = Vector(x, y, 0);
    return true;
}

float znajdzKatMiedzyPlaszczyznami(Plaszczyzna& pA, Plaszczyzna& pB) {
    float dotProdukt = pA.normal.dotProduct(pB.normal);
    float dlugoscA = pA.normal.length();
    float dlugoscB = pB.normal.length();

    if (dlugoscA < EPSILON || dlugoscB < EPSILON) return 0.0f;

    float cosinusKata = abs(dotProdukt) / (dlugoscA * dlugoscB);

    float katRad = acos(cosinusKata);
    return katRad * 180.0f / M_PI;
}

bool znajdzPrzeciecieOdcinkow(Vector& a1, Vector& a2, Vector& b1, Vector& b2, Vector& punktPrzeciecia) {
    Prosta prostaA = {a1, a2 - a1};
    Prosta prostaB = {b1, b2 - b1};

    // Sprawdź, czy nieskończone proste w ogóle się przecinają.
    if (!znajdzPrzeciecieProstych(prostaA, prostaB, punktPrzeciecia)) {
        return false; // Proste są równoległe lub skośne, więc odcinki się nie przetną.
    }

    // Sprawdź, czy punkt przecięcia leży na odcinku A.
    // Punkt P leży na odcinku A1-A2, jeśli wektor A1->P jest krótszy
    // niż A1->A2 i ma ten sam kierunek.
    // Prostszy test: czy t z równania P = A1 + t*(A2-A1) jest w zakresie [0, 1].
    float t;
    Vector vA = a2 - a1;
    if (abs(vA.x) > EPSILON) t = (punktPrzeciecia.x - a1.x) / vA.x;
    else if (abs(vA.y) > EPSILON) t = (punktPrzeciecia.y - a1.y) / vA.y;
    else t = (punktPrzeciecia.z - a1.z) / vA.z;

    if (t < 0.0f || t > 1.0f) {
        return false; // Punkt leży na prostej A, ale poza odcinkiem.
    }

    // Sprawdź, czy punkt przecięcia leży na odcinku B.
    float s;
    Vector vB = b2 - b1;
    if (abs(vB.x) > EPSILON) s = (punktPrzeciecia.x - b1.x) / vB.x;
    else if (abs(vB.y) > EPSILON) s = (punktPrzeciecia.y - b1.y) / vB.y;
    else s = (punktPrzeciecia.z - b1.z) / vB.z;

    if (s < 0.0f || s > 1.0f) {
        return false; // Punkt leży na prostej B, ale poza odcinkiem.
    }

    // Jeśli oba testy przeszły, odcinki się przecinają.
    return true;
}