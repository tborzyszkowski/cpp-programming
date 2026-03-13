#include <iostream>
#include "Circle.h"

// ============================================================
// Demonstracja: stałe metody i obiekty (const)
// Kompilacja: g++ -std=c++17 -o constdemo src/main.cpp
// ============================================================

int main() {
    std::cout << "=== 1. Zwykły obiekt – const i not-const metody ===\n";
    Circle c1(5.0, "blue");
    std::cout << c1.toString() << "\n";
    std::cout << "Area: " << c1.area() << "\n";

    c1.setRadius(7.0);       // OK – c1 nie jest const
    c1.setColor("green");
    std::cout << "Po zmianie: " << c1.toString() << "\n";
    c1.scale(2.0);
    std::cout << "Po scale(2): " << c1.toString() << "\n";

    std::cout << "\n=== 2. Obiekt CONST – tylko const metody ===\n";
    const Circle c2(10.0, "red");
    std::cout << c2.toString() << "\n";
    std::cout << "Area: " << c2.area() << "\n";
    std::cout << "Circumference: " << c2.circumference() << "\n";
    // c2.setRadius(20.0);   // BŁĄD KOMPILACJI: setRadius nie jest const!
    // c2.scale(3.0);        // BŁĄD KOMPILACJI: scale nie jest const!

    std::cout << "\n=== 3. Przekazanie przez const ref ===\n";
    printCircleInfo(c1);     // wywołuje tylko const metody
    printCircleInfo(c2);

    std::cout << "\n=== 4. Wskaźnik na const vs const wskaźnik ===\n";
    Circle c3(3.0);
    const Circle* ptrToConst = &c3;      // wskaźnik do stałego obiektu
    Circle* const constPtr   = &c3;      // stały wskaźnik do obiektu

    std::cout << ptrToConst->toString() << "\n";
    // ptrToConst->setRadius(5.0);        // BŁĄD
    constPtr->setRadius(5.0);            // OK – constPtr sam jest stały, nie obiekt
    std::cout << constPtr->toString()    << "\n";

    std::cout << "\n=== 5. static constexpr ===\n";
    std::cout << "Circle::PI = " << Circle::PI << "\n";

    return 0;
}
