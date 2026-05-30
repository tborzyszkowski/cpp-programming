#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <cmath>

// ============================================================
// Demonstracja: Moduły C++20
//
// UWAGA: Ze względu na różne wsparcie kompilatorów dla modułów,
// ten plik symuluje koncepcje modułów przez namespace'y i klasy.
// Komentarze pokazują, jak wyglądałby kod z pełnymi modułami C++20.
//
// Aby przetestować prawdziwe moduły, użyj MSVC 2022 lub GCC 14+
// z flagą -fmodules-ts (GCC) lub /experimental:module (MSVC).
// ============================================================
// Kompilacja (symulacja):
//   g++ -std=c++20 -o modules main.cpp && ./modules
// ============================================================

// ============================================================
// Symulacja modułu "math" – to co byłoby w math.cppm:
// ============================================================
//
//   export module math;
//
//   export double kwadrat(double x) { return x * x; }
//   export double szescian(double x) { return x * x * x; }
//   export class Kalkulator { ... };
//
//   // Nieeksportowane – prywatne implementacje:
//   double impl_pomocnicza(double x) { return x * 2.0; }  // ukryta!

namespace math_module {   // symulacja "export module math"
    double kwadrat(double x) { return x * x; }
    double szescian(double x) { return x * x * x; }
    double pierwiastek(double x) { return std::sqrt(x); }

    class Kalkulator {
        std::vector<double> historia_;
    public:
        double dodaj(double a, double b) {
            double wynik = a + b;
            historia_.push_back(wynik);
            return wynik;
        }
        double odejmij(double a, double b) {
            double wynik = a - b;
            historia_.push_back(wynik);
            return wynik;
        }
        const std::vector<double>& historia() const { return historia_; }
    };

    // "Nieeksportowana" implementacja wewnętrzna:
    // (w prawdziwym module byłaby za module :private)
    namespace priv {
        double impl_pomocnicza(double x) { return x * 2.0; }
    }
}

// ============================================================
// Symulacja modułu "geometry" z partycjami
// ============================================================
//
//   Partycja shapes (geometry-shapes.cppm):
//     export module geometry:shapes;
//     export struct Kolo { double r; };
//     export struct Prostokat { double a, b; };
//
//   Partycja algorithms (geometry-algorithms.cppm):
//     export module geometry:algorithms;
//     import :shapes;
//     export double pole(const Kolo& k);
//
//   Główna jednostka (geometry.cppm):
//     export module geometry;
//     export import :shapes;
//     export import :algorithms;

namespace geometry_module {
    namespace shapes {
        struct Kolo       { double r; };
        struct Prostokat  { double a, b; };
        struct Trojkat    { double a, b, c; };
    }

    namespace algorithms {
        double pole(const shapes::Kolo& k) {
            return 3.14159 * k.r * k.r;
        }
        double pole(const shapes::Prostokat& p) {
            return p.a * p.b;
        }
        double pole(const shapes::Trojkat& t) {
            double s = (t.a + t.b + t.c) / 2;
            return std::sqrt(s * (s-t.a) * (s-t.b) * (s-t.c));
        }
        double obwod(const shapes::Kolo& k) { return 2 * 3.14159 * k.r; }
        double obwod(const shapes::Prostokat& p) { return 2 * (p.a + p.b); }
    }
}

// ============================================================
// Demo 1: Porównanie #include vs import (konceptualne)
// ============================================================
void demo_porownanie() {
    std::cout << "=== Nagłówki vs Moduły – konceptualne porównanie ===\n";

    // Styl nagłówkowy (symulowany):
    //   #include "math.h"   // kopiuje CAŁY plik, włącznie z impl.
    //   kwadrat(5.0);       // OK, ale też widzisz impl_pomocnicza itp.

    // Styl modułowy (symulowany przez namespace):
    using namespace math_module;

    std::cout << "kwadrat(5.0) = "    << kwadrat(5.0)    << "\n";
    std::cout << "szescian(3.0) = "   << szescian(3.0)   << "\n";
    std::cout << "pierwiastek(16.0) = " << pierwiastek(16.0) << "\n";

    // impl_pomocnicza jest "ukryta" w priv:: – nie wycieka do użytkownika
    // (w prawdziwym module w ogóle nie byłaby dostępna!)
    std::cout << "(prywatna impl_pomocnicza jest ukryta)\n";
}

// ============================================================
// Demo 2: Kalkulator z historią
// ============================================================
void demo_kalkulator() {
    std::cout << "\n=== Kalkulator z historią (moduł math) ===\n";

    math_module::Kalkulator k;
    std::cout << "3 + 4 = " << k.dodaj(3, 4)     << "\n";
    std::cout << "10 - 3 = " << k.odejmij(10, 3) << "\n";
    std::cout << "7.5 + 2.5 = " << k.dodaj(7.5, 2.5) << "\n";

    std::cout << "Historia operacji: ";
    for (double v : k.historia()) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================
// Demo 3: Partycje modułu geometry
// ============================================================
void demo_partycje() {
    std::cout << "\n=== Partycje modułu geometry ===\n";

    using namespace geometry_module;

    shapes::Kolo      k{5.0};
    shapes::Prostokat p{3.0, 4.0};
    shapes::Trojkat   t{3.0, 4.0, 5.0};

    std::cout << "Kolo r=5:          pole=" << algorithms::pole(k)
              << " obwod=" << algorithms::obwod(k) << "\n";
    std::cout << "Prostokat 3x4:     pole=" << algorithms::pole(p)
              << " obwod=" << algorithms::obwod(p) << "\n";
    std::cout << "Trojkat 3-4-5:     pole=" << algorithms::pole(t) << "\n";
}

// ============================================================
// Demo 4: Zalety modułów – makra nie wyciekają
// ============================================================
void demo_makra() {
    std::cout << "\n=== Makra i moduły ===\n";

    // W systemie nagłówkowym:
    // Jeśli math.h definiuje: #define PI 3.14
    // To PI jest widoczne WSZĘDZIE po #include "math.h"
    // To powoduje kolizje i trudne do znalezienia błędy!

    // W systemie modułów:
    // Makra zdefiniowane wewnątrz modułu NIE wyciekają na zewnątrz
    // (To fundamentalna własność – moduły nie są pre-processorem)

    std::cout << "Moduł może wewnętrznie używać makr, ale:\n";
    std::cout << "- Nie wyciekają do importerów\n";
    std::cout << "- Brak kolizji z makrami użytkownika\n";
    std::cout << "- Deterministyczna kompilacja\n";
}

// ============================================================
// Demo 5: Szybkość kompilacji – symulacja benchmarku
// ============================================================
void demo_szybkosc() {
    std::cout << "\n=== Szybkość kompilacji (teoria) ===\n";

    struct BenchmarkEntry {
        std::string nazwa;
        int czas_nagłówki_ms;  // typowe wartości
        int czas_modulów_ms;
    };

    std::vector<BenchmarkEntry> benchmarks = {
        {"Mały projekt (10 plików)",     500,   150},
        {"Średni projekt (100 plików)",  5000,  1200},
        {"Duży projekt (1000 plików)",   60000, 12000},
        {"Inkrementalna (1 zmiana)",     2000,  100},
    };

    std::cout << std::left;
    std::cout << "Scenariusz                      Nagłówki  Moduły  Poprawa\n";
    std::cout << std::string(65, '-') << "\n";
    for (const auto& b : benchmarks) {
        double poprawa = (1.0 - (double)b.czas_modulów_ms / b.czas_nagłówki_ms) * 100;
        std::cout << b.nazwa;
        // padding
        for (int i = b.nazwa.size(); i < 32; ++i) std::cout << ' ';
        std::cout << b.czas_nagłówki_ms << "ms    "
                  << b.czas_modulów_ms << "ms    "
                  << (int)poprawa << "%\n";
    }
    std::cout << "(Wartości szacunkowe na podstawie benchmarków społeczności C++)\n";
}

int main() {
    demo_porownanie();
    demo_kalkulator();
    demo_partycje();
    demo_makra();
    demo_szybkosc();
    return 0;
}
