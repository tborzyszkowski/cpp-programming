#include <iostream>
#include "Car.h"

// ============================================================
// Demonstracja konstruktorów i destruktorów klasy Car
// Kompilacja: g++ -std=c++17 -o constructors main.cpp
// ============================================================

void scopeDemo() {
    std::cout << "\n--- Obiekty w zakresie (na stosie) ---\n";
    Car c1("Fiat", "125p", 1975, 9.5);
    Car c2("Toyota", "Corolla");   // konstruktor 2-arg (delegujący)
    std::cout << "c1 = " << c1.toString() << "\n";
    std::cout << "c2 = " << c2.toString() << "\n";
}  // ← tu automatycznie wywoływane są destruktory c1 i c2

void copyDemo() {
    std::cout << "\n--- Kopia konstruktorem kopiującym ---\n";
    Car original("BMW", "M3", 2022, 11.0);
    original.addService(2023);

    Car copy1(original);            // konstruktor kopiujący
    Car copy2 = original;           // też konstruktor kopiujący (nie operator=)
    Car copy3("placeholder", "x");
    copy3 = original;               // operator= (przypisanie)

    std::cout << "original: " << original.toString() << "\n";
    std::cout << "copy1:    " << copy1.toString()    << "\n";
    std::cout << "copy2:    " << copy2.toString()    << "\n";
    std::cout << "copy3:    " << copy3.toString()    << "\n";
}

int main() {
    std::cout << "=== Demo konstruktorów i destruktorów ===\n";

    std::cout << "\n[1] Konstruktor domyślny\n";
    Car c0;
    std::cout << "c0 = " << c0.toString() << "\n";

    std::cout << "\n[2] Konstruktor parametryczny\n";
    Car c1("Skoda", "Octavia", 2020, 7.2);
    std::cout << "c1 = " << c1.toString() << "\n";

    std::cout << "\n[3] Obiekty w zakresie – destruktory przy wyjściu\n";
    scopeDemo();
    std::cout << "Po powrocie z scopeDemo()\n";

    std::cout << "\n[4] Konstruktor kopiujący\n";
    copyDemo();

    std::cout << "\n[5] Obiekt na stercie\n";
    Car* heapCar = new Car("Mercedes", "C-Class", 2023);
    std::cout << "heapCar = " << heapCar->toString() << "\n";
    delete heapCar;          // ← destruktor wywołany ręcznie
    heapCar = nullptr;

    std::cout << "\n=== Koniec main – destruktory c0, c1 ===\n";
    return 0;
}
