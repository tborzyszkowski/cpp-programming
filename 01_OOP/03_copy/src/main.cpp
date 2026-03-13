#include <iostream>
#include "Buffer.h"

// ============================================================
// Demonstracja: Kopia płytka vs. kopia głęboka
// Kompilacja: g++ -std=c++17 -o copy main.cpp
// ============================================================

void demonstrateShallowCopy() {
    std::cout << "\n======= KOPIA PŁYTKA (nieprawidłowa!) =======\n";

    ShallowBuffer buf1(5);
    buf1.print("buf1 (oryginał)");

    // Kopia płytka – kompilator kopiuje wskaźnik, NIE dane!
    ShallowBuffer buf2 = buf1;     // wywołuje domyślny konstruktor kopiujący
    buf2.print("buf2 (kopia płytka)");

    std::cout << "Adres buf1.data_: " << buf1.data_ << "\n";
    std::cout << "Adres buf2.data_: " << buf2.data_ << "\n";
    std::cout << ">>> Oba obiekty wskazują na TEN SAM blok pamięci!\n";

    buf2.set(0, 999);             // zmiana przez buf2...
    buf1.print("buf1 po zmianie buf2[0]=999");  // ...widoczna w buf1!

    std::cout << "Po wyjściu ze scope: PODWÓJNY DELETE → niezdefiniowane zachowanie!\n";
    // BUG: oba destruktory wywołają delete[] na tym samym wskaźniku
}

void demonstrateDeepCopy() {
    std::cout << "\n======= KOPIA GŁĘBOKA (prawidłowa) =======\n";

    DeepBuffer buf1(5);
    buf1.print("buf1 (oryginał)");

    // Kopia głęboka – własny konstruktor kopiujący alokuje NOWY blok
    DeepBuffer buf2 = buf1;
    buf2.print("buf2 (kopia głęboka)");

    std::cout << "Adres buf1.data_: " << buf1.data_ << "\n";
    std::cout << "Adres buf2.data_: " << buf2.data_ << "\n";
    std::cout << ">>> Obiekty mają RÓŻNE, niezależne bloki!\n";

    buf2.set(0, 999);             // zmiana przez buf2...
    buf1.print("buf1 po buf2.set(0,999)");  // ...NIE widoczna w buf1!
    buf2.print("buf2 po buf2.set(0,999)");

    std::cout << "Destruktory wywołają delete[] bezpiecznie na różnych blokach.\n";
}

void demonstrateAssignment() {
    std::cout << "\n======= OPERATOR PRZYPISANIA (deep) =======\n";
    DeepBuffer buf1(3);
    DeepBuffer buf2(3);
    buf1.set(0, 100); buf1.set(1, 200); buf1.set(2, 300);
    buf2.set(0, 10);  buf2.set(1, 20);  buf2.set(2, 30);

    buf1.print("buf1 przed przypisaniem");
    buf2.print("buf2 przed przypisaniem");

    buf2 = buf1;    // operator=

    buf1.print("buf1 po przypisaniu");
    buf2.print("buf2 po przypisaniu");

    buf1.set(0, 999);
    buf1.print("buf1 po buf1.set(0,999)");
    buf2.print("buf2 (niezależna kopia)");
}

int main() {
    // Komentujemy wywołanie ShallowCopy bo prowadzi do UB (double free)
    // demonstrateShallowCopy();

    demonstrateDeepCopy();
    demonstrateAssignment();
    return 0;
}
