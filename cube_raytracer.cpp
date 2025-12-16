#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Vector.h"
#include "Matrix4x4.h"
#include "mathFunctions.h"

using namespace std;

// Struktura reprezentująca sześcian przez 6 płaszczyzn
struct Cube {
    Vector center;
    float size;

    Cube(Vector c, float s) : center(c), size(s) {}

    bool intersect(const Prosta& ray, float& tMin) {
        float halfSize = size / 2.0f;

        float xMin = center.x - halfSize;
        float xMax = center.x + halfSize;
        float yMin = center.y - halfSize;
        float yMax = center.y + halfSize;
        float zMin = center.z - halfSize;
        float zMax = center.z + halfSize;

        float tNear = -INFINITY;
        float tFar = INFINITY;

        if (fabs(ray.kierunek.x) > EPSILON) {
            float t1 = (xMin - ray.punkt.x) / ray.kierunek.x;
            float t2 = (xMax - ray.punkt.x) / ray.kierunek.x;
            if (t1 > t2) swap(t1, t2);
            tNear = max(tNear, t1);
            tFar = min(tFar, t2);
        } else {
            if (ray.punkt.x < xMin || ray.punkt.x > xMax)
                return false;
        }

        if (fabs(ray.kierunek.y) > EPSILON) {
            float t1 = (yMin - ray.punkt.y) / ray.kierunek.y;
            float t2 = (yMax - ray.punkt.y) / ray.kierunek.y;
            if (t1 > t2) swap(t1, t2);
            tNear = max(tNear, t1);
            tFar = min(tFar, t2);
        } else {
            if (ray.punkt.y < yMin || ray.punkt.y > yMax)
                return false;
        }

        if (fabs(ray.kierunek.z) > EPSILON) {
            float t1 = (zMin - ray.punkt.z) / ray.kierunek.z;
            float t2 = (zMax - ray.punkt.z) / ray.kierunek.z;
            if (t1 > t2) swap(t1, t2);
            tNear = max(tNear, t1);
            tFar = min(tFar, t2);
        } else {
            if (ray.punkt.z < zMin || ray.punkt.z > zMax)
                return false;
        }

        if (tNear > tFar || tFar < 0)
            return false;

        tMin = (tNear > 0) ? tNear : tFar;
        return true;
    }
};

struct Camera {
    Vector position;
    float angleY;      // Kąt obrotu wokół osi Y
    float angleX;      // Kąt obrotu wokół osi X
    float distance;    // Odległość od centrum
    float fov;         // Field of view w stopniach

    Camera() : angleY(0), angleX(20), distance(5.0f), fov(60.0f) {
        updatePosition();
    }

    void updatePosition() {
        float radY = angleY * M_PI / 180.0f;
        float radX = angleX * M_PI / 180.0f;

        position.x = distance * cos(radX) * sin(radY);
        position.y = distance * sin(radX);
        position.z = distance * cos(radX) * cos(radY);
    }

    void rotate(float deltaY, float deltaX) {
        angleY += deltaY;
        angleX += deltaX;

        if (angleX > 89.0f) angleX = 89.0f;
        if (angleX < -89.0f) angleX = -89.0f;

        updatePosition();
    }

    void zoom(float delta) {
        distance += delta;
        if (distance < 1.0f) distance = 1.0f;
        if (distance > 20.0f) distance = 20.0f;
        updatePosition();
    }
};

void renderScene(const Camera& camera, Cube& cube, int width, int height) {
    vector<vector<char>> screen(height, vector<char>(width, '.'));

    // Kierunek patrzenia kamery (do centrum układu współrzędnych)
    Vector lookAt(0, 0, 0);
    Vector forward = lookAt - camera.position;
    forward.normalize();

    // Wektor "góra" (up vector)
    Vector up(0, 1, 0);

    // Oblicz wektory right i actualUp dla płaszczyzny obrazu
    Vector right = forward.cross(up);
    right.normalize();

    Vector actualUp = right.cross(forward);
    actualUp.normalize();

    // Oblicz wymiary płaszczyzny obrazu
    float aspectRatio = (float)width / height;
    float fovRad = camera.fov * M_PI / 180.0f;
    float planeHeight = 2.0f * tan(fovRad / 2.0f);
    float planeWidth = planeHeight * aspectRatio;

    // Renderuj każdy piksel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Normalizuj współrzędne do zakresu [-1, 1]
            float u = (2.0f * x / width - 1.0f) * planeWidth / 2.0f;
            float v = (1.0f - 2.0f * y / height) * planeHeight / 2.0f;

            // Oblicz kierunek promienia
            Vector rayDir = forward + right * u + actualUp * v;
            rayDir.normalize();

            // Stwórz promień
            Prosta ray = {camera.position, rayDir};

            // Sprawdź przecięcie z sześcianem
            float t;
            if (cube.intersect(ray, t)) {
                screen[y][x] = '0';
            }
        }
    }

    // Wyświetl ekran
    cout << "\nKamera: pozycja=[" << camera.position.x << ", "
         << camera.position.y << ", " << camera.position.z << "]"
         << " angleY=" << camera.angleY
         << " angleX=" << camera.angleX
         << " distance=" << camera.distance << "\n\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << screen[y][x];
        }
        cout << endl;
    }
}

void renderCubeRaytracer() {
    const int WIDTH = 60;
    const int HEIGHT = 60;

    Cube cube(Vector(0, 0, 0), 2.0f);

    Camera camera;

    char command;
    bool running = true;

    cout << "=== RAY TRACER SZESCIANU ===\n";
    cout << "Sterowanie:\n";
    cout << "  w/s - obrot w gore/dol\n";
    cout << "  a/d - obrot w lewo/prawo\n";
    cout << "  +/- - przybliz/oddal\n";
    cout << "  r   - renderuj\n";
    cout << "  q   - wyjscie\n\n";

    renderScene(camera, cube, WIDTH, HEIGHT);

    while (running) {
        cout << "\nPodaj polecenie: ";
        cin >> command;

        switch (command) {
            case 'w':
                camera.rotate(0, 10);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case 's':
                camera.rotate(0, -10);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case 'a':
                camera.rotate(-10, 0);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case 'd':
                camera.rotate(10, 0);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case '+':
                camera.zoom(-0.5f);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case '-':
                camera.zoom(0.5f);
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case 'r':
                renderScene(camera, cube, WIDTH, HEIGHT);
                break;
            case 'q':
                running = false;
                cout << "Koniec ray tracera.\n";
                break;
            default:
                cout << "Nieznane polecenie!\n";
        }
    }
}