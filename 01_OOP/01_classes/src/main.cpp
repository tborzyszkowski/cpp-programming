#include <iostream>
#include "Point.h"
#include "Rectangle.h"

// ============================================================
// Demonstracja: Definicja klasy i obiektu w C++
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o classes main.cpp
// ============================================================

int main() {
    std::cout << "=== Tworzenie obiektów klasy Point ===\n";
    // Obiekt na stosie – konstruktor domyślny
    Point p0;
    std::cout << "p0 = " << p0.toString() << "\n";

    // Obiekt na stosie – konstruktor parametryczny
    Point p1(3.0, 4.0);
    std::cout << "p1 = " << p1.toString() << "\n";
    std::cout << "Odległość p1 od O = " << p1.distanceFromOrigin() << "\n";

    // Modyfikacja przez settery
    p0.setX(1.0);
    p0.setY(2.0);
    std::cout << "p0 po modyfikacji = " << p0.toString() << "\n";

    std::cout << "\n=== Tworzenie obiektów klasy Rectangle ===\n";
    Point tl(0.0, 5.0);
    Point br(4.0, 0.0);
    Rectangle rect(tl, br, "blue");
    std::cout << "Prostokąt: " << rect.toString() << "\n";
    std::cout << "Pole: "          << rect.area()      << "\n";
    std::cout << "Obwód: "         << rect.perimeter() << "\n";

    // Zmiana właściwości
    rect.setColor("red");
    std::cout << "Po zmianie koloru: " << rect.toString() << "\n";

    std::cout << "\n=== Obiekt na stercie (new/delete) ===\n";
    Point* heapPoint = new Point(7.0, 24.0);
    std::cout << "heapPoint = " << heapPoint->toString() << "\n";
    std::cout << "Odległość od O = " << heapPoint->distanceFromOrigin() << "\n";
    delete heapPoint;  // konieczne – brak GC w C++!
    heapPoint = nullptr;

    std::cout << "\n=== Koniec zakresu – destruktory wywołane automatycznie ===\n";
    return 0;
}
