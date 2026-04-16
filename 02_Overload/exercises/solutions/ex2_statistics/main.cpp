#include <iostream>
#include <iomanip>
#include "Statistics.h"

// ============================================================
// Rozwiązanie Zadania 2: Statistics – funktor z operator()
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex2_statistics main.cpp
// ============================================================

int main() {
    std::cout << std::fixed << std::setprecision(2);

    // --- Łańcuchowe dodawanie ---
    std::cout << "=== Łańcuchowe dodawanie ===\n";
    Statistics pomiary("Pomiary");
    pomiary(1.0)(2.0)(3.0)(4.0)(5.0);   // operator() × 5
    std::cout << pomiary << "\n\n";

    // --- Konwersja ---
    std::cout << "=== Konwersja ===\n";
    std::cout << "Jako double (średnia): " << static_cast<double>(pomiary) << "\n";
    std::cout << "Jako bool: " << std::boolalpha << static_cast<bool>(pomiary) << "\n";

    Statistics pusty("Pusty");
    std::cout << "Pusty jako bool: " << static_cast<bool>(pusty) << "\n\n";

    // --- Scalanie danych ---
    std::cout << "=== Scalanie danych ===\n";
    Statistics dzien1("Dzień 1");
    dzien1(1.0)(2.0)(3.0)(4.0)(5.0);

    Statistics dzien2("Dzień 2");
    dzien2(6.0)(7.0)(8.0);

    Statistics razem = dzien1 + dzien2;
    std::cout << razem << "\n";

    return 0;
}
