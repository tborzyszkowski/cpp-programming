#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Temperature.h"

using namespace temperature_literals;

// ============================================================
// Rozwiązanie Zadania 3: Temperature – konwersje i porównania
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex3_temperature main.cpp
// ============================================================

int main() {
    std::cout << std::fixed << std::setprecision(2);

    // --- Tworzenie ---
    std::cout << "=== Tworzenie ===\n";
    auto t1 = Temperature::fromCelsius(100.0);
    auto t2 = Temperature::fromCelsius(0.0);
    auto t3 = Temperature::fromFahrenheit(100.0);

    std::cout << "t1 = " << t1 << "\n";
    std::cout << "t2 = " << t2 << "\n";
    std::cout << "t3 = " << t3 << "    // 100°F\n\n";

    // --- Literały użytkownika ---
    std::cout << "=== Literały użytkownika ===\n";
    auto lc = 100.0_C;
    auto lf = 212.0_F;
    auto lk = 300.0_K;
    std::cout << "100.0_C = " << lc << "\n";
    std::cout << "212.0_F = " << lf << "\n";
    std::cout << "300.0_K = " << lk << "\n\n";

    // --- Porównania ---
    std::cout << "=== Porównania ===\n" << std::boolalpha;
    std::cout << "100°C > 0°C    ? " << (t1 > t2) << "\n";
    std::cout << "100°C == 212°F ? " << (t1 == lf) << "\n\n";

    // --- Arytmetyka ---
    std::cout << "=== Arytmetyka ===\n" << std::noboolalpha;
    std::cout << "t1 - t2 = " << (t1 - t2) << " (różnica w K)\n";
    std::cout << "t2 + 10 = " << (t2 + 10.0) << "\n\n";

    // --- Walidacja ---
    std::cout << "=== Walidacja ===\n";
    try {
        Temperature bad(-1.0);   // poniżej zera absolutnego!
    } catch (const std::invalid_argument& e) {
        std::cout << "Wyjątek: " << e.what() << "\n";
    }

    return 0;
}
