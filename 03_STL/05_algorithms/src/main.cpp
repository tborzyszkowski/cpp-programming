#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <string>
#include <functional>

// ============================================================
// Demonstracja: Algorytmy STL
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o algorithms main.cpp && ./algorithms
// ============================================================

void demo_niemodyfikujace() {
    std::cout << "=== Algorytmy niemodyfikujące ===\n";
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    std::cout << "Liczba 1: "  << std::count(v.begin(), v.end(), 1) << "\n";
    std::cout << "Liczba >5: " << std::count_if(v.begin(), v.end(),
        [](int x){ return x > 5; }) << "\n";

    std::cout << "Wszystkie > 0: " << std::boolalpha
        << std::all_of(v.begin(), v.end(), [](int x){ return x > 0; }) << "\n";

    auto [lo, hi] = std::minmax_element(v.begin(), v.end());
    std::cout << "min=" << *lo << " max=" << *hi << "\n";

    auto it = std::find_if(v.begin(), v.end(), [](int x){ return x > 7; });
    if (it != v.end())
        std::cout << "Pierwszy > 7: " << *it << "\n";
}

void demo_modyfikujace() {
    std::cout << "\n=== Algorytmy modyfikujące ===\n";
    std::vector<int> v(6);
    std::iota(v.begin(), v.end(), 1);  // {1,2,3,4,5,6}

    // Kwadraty
    std::vector<int> kwadraty(v.size());
    std::transform(v.begin(), v.end(), kwadraty.begin(),
        [](int x){ return x * x; });
    std::cout << "Kwadraty: ";
    for (int x : kwadraty) std::cout << x << " ";
    std::cout << "\n";

    // Odwróć
    std::reverse(v.begin(), v.end());
    std::cout << "Odwrócony: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // Replace
    std::replace_if(v.begin(), v.end(),
        [](int x){ return x % 2 == 0; }, 0);
    std::cout << "Parzyste→0: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

void demo_sortowanie() {
    std::cout << "\n=== Sortowanie ===\n";
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::sort(v.begin(), v.end());
    std::cout << "Posortowany: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // Sortowanie obiektów
    struct Osoba { std::string imie; int wiek; };
    std::vector<Osoba> osoby = {{"Anna",30}, {"Bartek",25}, {"Celina",35}};
    std::sort(osoby.begin(), osoby.end(),
        [](const Osoba& a, const Osoba& b){ return a.wiek < b.wiek; });
    for (const auto& o : osoby)
        std::cout << o.imie << "(" << o.wiek << ") ";
    std::cout << "\n";
}

void demo_szukanie_posortowane() {
    std::cout << "\n=== Szukanie w posortowanym ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::cout << "Czy 5: " << std::boolalpha
        << std::binary_search(v.begin(), v.end(), 5) << "\n";

    auto lo = std::lower_bound(v.begin(), v.end(), 4);
    auto hi = std::upper_bound(v.begin(), v.end(), 7);
    std::cout << "Zakres [4,7]: ";
    for (auto it = lo; it != hi; ++it) std::cout << *it << " ";
    std::cout << "\n";
}

void demo_redukcja() {
    std::cout << "\n=== Redukcja ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5};

    int suma    = std::accumulate(v.begin(), v.end(), 0);
    int iloczyn = std::accumulate(v.begin(), v.end(), 1,
        std::multiplies<int>{});

    std::cout << "Suma: " << suma << "\n";       // 15
    std::cout << "Iloczyn: " << iloczyn << "\n"; // 120

    // Sumy prefiksowe
    std::vector<int> prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());
    std::cout << "Sumy prefiksowe: ";
    for (int x : prefix) std::cout << x << " ";  // 1 3 6 10 15
    std::cout << "\n";
}

void demo_erase_remove() {
    std::cout << "\n=== Idiom erase-remove ===\n";
    std::vector<int> v = {1, 2, 3, 4, 2, 5, 2, 6};

    // Usuń wszystkie 2
    v.erase(std::remove(v.begin(), v.end(), 2), v.end());
    std::cout << "Po usunięciu 2: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // C++20: std::erase_if
    std::erase_if(v, [](int x){ return x % 2 == 0; });
    std::cout << "Po usunięciu parzystych: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

void demo_ranges() {
    std::cout << "\n=== C++20 Ranges ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto pipeline = v
        | std::views::filter([](int x){ return x % 2 == 0; })
        | std::views::transform([](int x){ return x * x; })
        | std::views::take(3);

    std::cout << "3 pierwsze kwadraty parzystych: ";
    for (int x : pipeline) std::cout << x << " ";  // 4 16 36
    std::cout << "\n";
}

int main() {
    demo_niemodyfikujace();
    demo_modyfikujace();
    demo_sortowanie();
    demo_szukanie_posortowane();
    demo_redukcja();
    demo_erase_remove();
    demo_ranges();
    return 0;
}
