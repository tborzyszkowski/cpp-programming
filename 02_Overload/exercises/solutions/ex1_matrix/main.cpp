#include <iostream>
#include <stdexcept>
#include "Matrix2x2.h"

// ============================================================
// Rozwiązanie Zadania 1: Matrix2x2 – operatory arytmetyczne
// i indeksowania
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex1_matrix main.cpp
// ============================================================

int main() {
    Matrix2x2 A(1, 2, 3, 4);
    Matrix2x2 B(5, 6, 7, 8);

    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n\n";

    std::cout << "A + B = " << (A + B) << "\n";
    std::cout << "A * B = " << (A * B) << "\n";
    std::cout << "A * 2 = " << (A * 2) << "\n\n";

    std::cout << "A[0][1] = " << A[0][1] << "      (operator[])\n";
    std::cout << "A(1, 0) = " << A(1, 0) << "      (operator())\n\n";

    std::cout << "det(A) = " << A.determinant() << "\n";
    std::cout << "A^T    = " << A.transposed()  << "\n\n";

    std::cout << std::boolalpha;
    std::cout << "A == A ? " << (A == A) << "\n";
    std::cout << "A == B ? " << (A == B) << "\n\n";

    // Kontrola zakresu w operator()
    try {
        A(3, 0);
    } catch (const std::out_of_range& e) {
        std::cout << "Wyjątek: " << e.what() << "\n";
    }

    return 0;
}
