#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>

// ============================================================
// Demonstracja: Ewolucja callables – od wskaźników do lambd
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o history main.cpp && ./history
// ============================================================

// ============================================================
// ETAP 1: Wskaźniki na funkcje (styl C)
// ============================================================
int porownaj_rosnaco(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

bool jest_parzysta(int x) { return x % 2 == 0; }

void demo_wskazniki() {
    std::cout << "=== Wskazniki na funkcje ===\n";

    int tab[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::qsort(tab, 9, sizeof(int), porownaj_rosnaco);

    std::cout << "Po qsort: ";
    for (int x : tab) std::cout << x << " ";
    std::cout << "\n";

    // std::count_if z funkcją (nie wskaźnikiem – ale styl C++98)
    std::vector<int> v(tab, tab + 9);
    int ile = std::count_if(v.begin(), v.end(), jest_parzysta);
    std::cout << "Parzyste: " << ile << "\n";

    // Ograniczenie: próg jest na stałe w funkcji
    // Nie można przestawić progu bez nowej funkcji
}

// ============================================================
// ETAP 2: Funktory (C++98)
// ============================================================
struct WiekszyNiz {
    int prog;
    explicit WiekszyNiz(int p) : prog(p) {}
    bool operator()(int x) const { return x > prog; }
};

struct Mnoznik {
    double wspolczynnik;
    explicit Mnoznik(double w) : wspolczynnik(w) {}
    double operator()(double x) const { return x * wspolczynnik; }
};

// Funktor sortujący malejąco według wartości bezwzględnej
struct ByAbsoluteValueDesc {
    bool operator()(int a, int b) const {
        return std::abs(a) > std::abs(b);
    }
};

void demo_funktory() {
    std::cout << "\n=== Funktory (C++98) ===\n";

    std::vector<int> v = {1, 3, 5, 7, 9, 11};

    // Elastyczny próg – przekazany przez konstruktor
    std::cout << "Ile > 5: " << std::count_if(v.begin(), v.end(), WiekszyNiz{5}) << "\n";
    std::cout << "Ile > 8: " << std::count_if(v.begin(), v.end(), WiekszyNiz{8}) << "\n";

    // Transformacja z parametrem
    std::vector<double> d = {1.0, 2.0, 3.0, 4.0};
    std::transform(d.begin(), d.end(), d.begin(), Mnoznik{2.5});
    std::cout << "Po x2.5: ";
    for (double x : d) std::cout << x << " ";
    std::cout << "\n";

    // Sortowanie niestandardowe
    std::vector<int> w = {-10, 3, -7, 1, 5, -2};
    std::sort(w.begin(), w.end(), ByAbsoluteValueDesc{});
    std::cout << "Malejaco |x|: ";
    for (int x : w) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// ETAP 3: std::bind (C++11, ale wzorowany na Boost.Bind)
// ============================================================
bool wiekszy_niz(int x, int prog) { return x > prog; }
double przeskaluj(double x, double skala, double przesuniecie) {
    return x * skala + przesuniecie;
}

void demo_bind() {
    std::cout << "\n=== std::bind (C++11) ===\n";

    std::vector<int> v = {1, 3, 5, 7, 9, 11};

    // Wiązanie drugiego argumentu (próg = 5)
    auto wieksze_od_5 = std::bind(wiekszy_niz, std::placeholders::_1, 5);
    std::cout << "Ile > 5 (bind): "
              << std::count_if(v.begin(), v.end(), wieksze_od_5) << "\n";

    // Wiązanie z dwoma wolnymi argumentami
    std::vector<double> d = {1.0, 2.0, 3.0, 4.0};
    auto skala2_przesuniecie3 = std::bind(przeskaluj, std::placeholders::_1, 2.0, 3.0);
    std::transform(d.begin(), d.end(), d.begin(), skala2_przesuniecie3);
    std::cout << "Po *2+3: ";
    for (double x : d) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "(Uwaga: std::bind jest trudny do czytania i optymalizacji)\n";
}

// ============================================================
// ETAP 4: Lambdy (C++11) – to samo, czytelniej
// ============================================================
void demo_lambdy() {
    std::cout << "\n=== Lambdy (C++11) ===\n";

    std::vector<int> v = {1, 3, 5, 7, 9, 11};

    int prog = 5;
    std::cout << "Ile > " << prog << " (lambda): "
              << std::count_if(v.begin(), v.end(),
                               [prog](int x){ return x > prog; }) << "\n";

    prog = 8;
    std::cout << "Ile > " << prog << " (lambda): "
              << std::count_if(v.begin(), v.end(),
                               [prog](int x){ return x > prog; }) << "\n";

    // Przekształcenie z parametrami
    std::vector<double> d = {1.0, 2.0, 3.0, 4.0};
    double skala = 2.0, przesuniecie = 3.0;
    std::transform(d.begin(), d.end(), d.begin(),
                   [skala, przesuniecie](double x){ return x * skala + przesuniecie; });
    std::cout << "Po *" << skala << "+" << przesuniecie << ": ";
    for (double x : d) std::cout << x << " ";
    std::cout << "\n";

    // Lambda zamiast ByAbsoluteValueDesc
    std::vector<int> w = {-10, 3, -7, 1, 5, -2};
    std::sort(w.begin(), w.end(),
              [](int a, int b){ return std::abs(a) > std::abs(b); });
    std::cout << "Malejaco |x| (lambda): ";
    for (int x : w) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// Podsumowanie – porównanie stylów
// ============================================================
void porownanie_stylów() {
    std::cout << "\n=== Porownanie: to samo zadanie, 4 style ===\n";
    std::cout << "Zlicz elementy > 5 w {1,3,5,7,9,11}:\n";

    std::vector<int> v = {1, 3, 5, 7, 9, 11};

    // Styl 1: wskaźnik na funkcję (próg hardcoded)
    std::cout << "  Funkcja:   " << std::count_if(v.begin(), v.end(), jest_parzysta)
              << " (to inny predykat – wskaznik nie moze miec progu)\n";

    // Styl 2: funktor
    std::cout << "  Funktor:   " << std::count_if(v.begin(), v.end(), WiekszyNiz{5}) << "\n";

    // Styl 3: bind
    std::cout << "  bind:      " << std::count_if(v.begin(), v.end(),
                                   std::bind(wiekszy_niz, std::placeholders::_1, 5)) << "\n";

    // Styl 4: lambda
    std::cout << "  Lambda:    " << std::count_if(v.begin(), v.end(),
                                   [](int x){ return x > 5; }) << "\n";
}

int main() {
    demo_wskazniki();
    demo_funktory();
    demo_bind();
    demo_lambdy();
    porownanie_stylów();
    return 0;
}
