#include <iostream>
#include "Car.h"

// ============================================================
// Demonstracja: zaprzyjaźnione funkcje i klasy
// Kompilacja: g++ -std=c++17 -o friend src/main.cpp
// ============================================================

int main() {
    Car car1("Fiat", "125p",  1975, 87500, 40.0);
    Car car2("Toyota", "Corolla", 2020, 15000, 90.0);

    std::cout << "car1: " << car1.toString() << "\n";
    std::cout << "car2: " << car2.toString() << "\n";

    // === Zaprzyjaźniona funkcja ===
    std::cout << "\n=== Zaprzyjaźniona funkcja ===\n";
    printServiceReport(car1, 350.0);   // dostęp do prywatnych pól!
    printServiceReport(car2, 120.0);

    // === Zaprzyjaźniona klasa ===
    std::cout << "\n=== Zaprzyjaźniona klasa Workshop ===\n";
    Workshop workshop("AutoSerwis Kowalski", 80.0);
    workshop.diagnose(car1);
    workshop.service(car1, 3.5);
    std::cout << "\nPo serwisie: " << car1.toString() << "\n";

    workshop.diagnose(car2);
    workshop.service(car2, 1.0);
    std::cout << "\nPo serwisie: " << car2.toString() << "\n";

    // Demonstracja: bez friend nie ma dostępu do prywatnych pól
    // car1.fuelLevel_;   // BŁĄD KOMPILACJI – prywatne!

    return 0;
}
