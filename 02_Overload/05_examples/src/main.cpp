#include <iostream>
#include "Fraction.h"

// ============================================================
// Demonstracja: klasa Fraction z pełnym zestawem operatorów
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o fraction main.cpp && ./fraction
// ============================================================

int main() {
    std::cout << "=== Tworzenie ułamków ===\n";
    Fraction a(1, 2);    // 1/2
    Fraction b(1, 3);    // 1/3
    Fraction c(2, 4);    // → normalizacja → 1/2
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "  (2/4 po normalizacji)\n";

    std::cout << "\n=== Arytmetyka ===\n";
    std::cout << "a + b = " << (a + b) << "\n";   // 5/6
    std::cout << "a - b = " << (a - b) << "\n";   // 1/6
    std::cout << "a * b = " << (a * b) << "\n";   // 1/6
    std::cout << "a / b = " << (a / b) << "\n";   // 3/2
    std::cout << "-a    = " << (-a)    << "\n";   // -1/2

    std::cout << "\n=== Przypisanie z działaniem ===\n";
    Fraction d(3, 4);
    d += a;
    std::cout << "d(3/4) += a(1/2): " << d << "\n";  // 5/4
    d -= Fraction(1, 4);
    std::cout << "d -= 1/4:         " << d << "\n";  // 1
    d *= Fraction(2, 3);
    std::cout << "d *= 2/3:         " << d << "\n";  // 2/3
    d /= Fraction(1, 3);
    std::cout << "d /= 1/3:         " << d << "\n";  // 2

    std::cout << "\n=== Porównania ===\n";
    std::cout << std::boolalpha;
    std::cout << "1/2 == 2/4: " << (a == c)              << "\n";  // true
    std::cout << "1/2 != 1/3: " << (a != b)              << "\n";  // true
    std::cout << "1/3 <  1/2: " << (b < a)               << "\n";  // true
    std::cout << "1/2 >  1/3: " << (a > b)               << "\n";  // true
    std::cout << "1/2 <= 1/2: " << (a <= c)              << "\n";  // true
    std::cout << "1/3 >= 1/2: " << (b >= a)              << "\n";  // false

    std::cout << "\n=== Konwersja na double ===\n";
    std::cout << "1/2 = " << a.toDouble() << "\n";   // 0.5
    std::cout << "1/3 = " << b.toDouble() << "\n";   // 0.3333...

    std::cout << "\n=== Wejście (wpisz ułamek, np. 3/4) ===\n";
    Fraction f;
    std::cin >> f;
    std::cout << "Wczytano: " << f << " = " << f.toDouble() << "\n";
    std::cout << "Podwójony: " << (f + f) << "\n";

    return 0;
}
