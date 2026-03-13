#include <iostream>
#include <cmath>
#include "Matrix2x2.h"

// ============================================================
// Rozwiązanie Zadania 2: Vector2D + Matrix2x2
// operator* jako friend obu klas jednocześnie
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex2_matrix main.cpp
// ============================================================

int main() {
    const double PI = std::acos(-1.0);

    Vector2D v(3.0, 4.0);
    std::cout << "v = " << v << "  |v| = " << v.length() << "\n";

    // Skalowanie – macierz diagonalna
    Matrix2x2 scale(2.0, 0.0, 0.0, 3.0);
    std::cout << "\nMacierz skalowania:\n" << scale << "\n";
    Vector2D vs = scale * v;
    std::cout << "scale * v = " << vs << "\n";

    // Rotacja o 90° (macierz: [0 -1; 1 0])
    Matrix2x2 rot90(0.0, -1.0, 1.0, 0.0);
    std::cout << "\nMacierz rotacji 90°:\n" << rot90 << "\n";
    Vector2D vr = rot90 * v;
    std::cout << "rot90 * v = " << vr << "\n";
    std::cout << "|rot90 * v| = " << vr.length()
              << "  (powinno równać się |v| = " << v.length() << ")\n";

    // Rotacja o dowolny kąt
    double angle = PI / 6.0;   // 30°
    Matrix2x2 rot30( std::cos(angle), -std::sin(angle),
                     std::sin(angle),  std::cos(angle));
    std::cout << "\nRotacja 30°: " << (rot30 * v) << "\n";

    // Mnożenie macierzy przez macierz, potem przez wektor
    Matrix2x2 combined = rot90 * scale;
    std::cout << "\nrot90 * scale:\n" << combined << "\n";
    std::cout << "(rot90 * scale) * v = " << (combined * v) << "\n";

    // Wyznacznik
    std::cout << "\ndet(scale) = " << scale.determinant() << "\n";
    std::cout << "det(rot90) = " << rot90.determinant() << "\n";

    return 0;
}
