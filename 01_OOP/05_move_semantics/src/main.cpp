#include <iostream>
#include <vector>
#include "Buffer.h"

// ============================================================
// Demonstracja: Move semantics i move constructor
// Kompilacja: g++ -std=c++17 -o move main.cpp
// ============================================================

// Funkcja zwracająca Buffer przez wartość – RVO lub move
Buffer createBuffer(const std::string& name, size_t size) {
    Buffer tmp(name, size);
    return tmp;   // kompilator użyje NRVO lub move, nie kopiowania
}

int main() {
    std::cout << "=== 1. Kopia vs. Move – różnica w alokacji ===\n";
    Buffer b1("alpha", 5);

    std::cout << "\n-- Kopia (nowa alokacja) --\n";
    Buffer b2 = b1;              // copy constructor
    b1.print();
    b2.print();

    std::cout << "\n-- Move (przejęcie zasobu) --\n";
    Buffer b3 = std::move(b1);   // move constructor
    b3.print();                  // b3 ma dane
    b1.print();                  // b1 jest "wyprany" (moved-from)

    std::cout << "\n=== 2. Move operator= ===\n";
    Buffer b4("beta", 3);
    Buffer b5("gamma", 3);
    b4.print();
    b5.print();
    b4 = std::move(b5);          // move assignment operator
    b4.print();
    b5.print();

    std::cout << "\n=== 3. std::vector i move (bez zbędnych kopii) ===\n";
    std::vector<Buffer> buffers;
    buffers.reserve(3);   // zarezerwuj miejsce – unika realokacji

    std::cout << "push_back temporaries (move):\n";
    buffers.push_back(Buffer("v1", 4));   // tymczasowy → move
    buffers.push_back(Buffer("v2", 4));
    buffers.push_back(Buffer("v3", 4));

    for (const auto& b : buffers)
        b.print();

    std::cout << "\n=== 4. Funkcja zwracająca Buffer – NRVO/RVO ===\n";
    Buffer b6 = createBuffer("returned", 3);
    b6.print();

    std::cout << "\n=== Koniec main – destruktory ===\n";
    return 0;
}
