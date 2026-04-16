#include <iostream>
#include "Vec3d.h"

// ============================================================
// Demonstracja: klasa Vec3d – wektor 3D z operatorami
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o vec3d main.cpp && ./vec3d
// ============================================================

int main() {
    std::cout << "=== Tworzenie wektorów ===\n";
    Vec3d v1(1, 2, 3);
    Vec3d v2(4, 5, 6);
    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";

    std::cout << "\n=== Dodawanie i odejmowanie ===\n";
    std::cout << "v1 + v2 = " << (v1 + v2) << "\n";  // (X: 5, Y: 7, Z: 9)
    std::cout << "v2 - v1 = " << (v2 - v1) << "\n";  // (X: 3, Y: 3, Z: 3)

    std::cout << "\n=== Iloczyn wektorowy (v1 x v2) ===\n";
    // v1=(1,2,3), v2=(4,5,6)
    // x: 2*6 - 3*5 = 12 - 15 = -3
    // y: 3*4 - 1*6 =  12 - 6  =  6   → uwaga: z_*o.x_ - x_*o.z_ = 3*4 - 1*6 = 6
    // z: 1*5 - 2*4 =   5 - 8  = -3
    std::cout << "v1 * v2 = " << (v1 * v2) << "\n";  // (X: -3, Y: 6, Z: -3)

    std::cout << "\n=== Skalowanie ===\n";
    std::cout << "v1 * 3  = " << (v1 * 3)  << "\n";  // (X: 3, Y: 6, Z: 9)
    std::cout << "2 * v2  = " << (2 * v2)  << "\n";  // (X: 8, Y: 10, Z: 12)

    std::cout << "\n=== Inkrementacja prefix i postfix ===\n";
    Vec3d v3(0, 0, 0);
    std::cout << "v3        = " << v3   << "\n";
    std::cout << "++v3      = " << ++v3 << "  (v3 teraz: " << v3 << ")\n";
    std::cout << "v3++      = " << v3++ << "  (stary stan; v3 teraz: " << v3 << ")\n";
    std::cout << "--v3      = " << --v3 << "  (v3 teraz: " << v3 << ")\n";
    std::cout << "v3--      = " << v3-- << "  (stary stan; v3 teraz: " << v3 << ")\n";

    std::cout << "\n=== Porównania ===\n";
    std::cout << std::boolalpha;
    std::cout << "v1 == v1: " << (v1 == v1) << "\n";  // true
    std::cout << "v1 == v2: " << (v1 == v2) << "\n";  // false
    std::cout << "v1 != v2: " << (v1 != v2) << "\n";  // true

    std::cout << "\n=== Wczytywanie wektora przez użytkownika ===\n";
    Vec3d v4;
    std::cout << "Podaj wektor v4:\n";
    std::cin >> v4;
    std::cout << "Podany wektor: " << v4 << "\n";
    std::cout << "v1 + v4    = " << (v1 + v4) << "\n";
    std::cout << "v1 * v4    = " << (v1 * v4) << "\n";
    std::cout << "v4 == v1   = " << (v4 == v1) << "\n";

    return 0;
}
