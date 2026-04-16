#include <iostream>
#include "Complex.h"

// ============================================================
// Demonstracja: Składnia i zasady przeciążania operatorów
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o complex main.cpp && ./complex
// ============================================================

int main() {
    std::cout << "=== Tworzenie liczb zespolonych ===\n";
    Complex c1(3.0, 4.0);
    Complex c2(1.0, -2.0);
    std::cout << "c1 = " << c1 << "\n";   // 3 + 4i
    std::cout << "c2 = " << c2 << "\n";   // 1 - 2i

    std::cout << "\n=== Operatory arytmetyczne ===\n";
    std::cout << "c1 + c2 = " << (c1 + c2) << "\n";  // 4 + 2i
    std::cout << "c1 - c2 = " << (c1 - c2) << "\n";  // 2 + 6i
    std::cout << "c1 * c2 = " << (c1 * c2) << "\n";  // 11 - 2i
    std::cout << "c1 * 2  = " << (c1 * 2.0)<< "\n";  // 6 + 8i

    std::cout << "\n=== Operator jednoargumentowy ===\n";
    std::cout << "-c1     = " << (-c1)  << "\n";   // -3 - 4i
    std::cout << "+c1     = " << (+c1)  << "\n";   //  3 + 4i
    std::cout << "|c1|    = " << c1.abs() << "\n"; // 5

    std::cout << "\n=== Operatory przypisania z działaniem ===\n";
    Complex c3(0.0, 0.0);
    c3 += c1;
    std::cout << "c3 += c1: " << c3 << "\n";   // 3 + 4i
    c3 -= c2;
    std::cout << "c3 -= c2: " << c3 << "\n";   // 2 + 6i
    c3 *= Complex(2.0, 0.0);
    std::cout << "c3 *= 2: "  << c3 << "\n";   // 4 + 12i

    // Łańcuchowanie
    Complex c4(1.0, 0.0);
    c4 += c1 += c2;   // c1 += c2 modyfikuje c1, wynik idzie do c4
    std::cout << "c4 po łańcuchowaniu: " << c4 << "\n";

    std::cout << "\n=== Porównania ===\n";
    std::cout << std::boolalpha;
    std::cout << "c1 == c1: " << (c1 == c1) << "\n"; // true
    std::cout << "c1 != c2: " << (c1 != c2) << "\n"; // true

    std::cout << "\n=== Wejście (strumień) ===\n";
    Complex cinput;
    std::cout << "Podaj liczbę zespoloną:\n";
    std::cin >> cinput;
    std::cout << "Podano: " << cinput << "\n";
    std::cout << "|podano| = " << cinput.abs() << "\n";

    return 0;
}
