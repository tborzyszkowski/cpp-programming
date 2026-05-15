#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

// ============================================================
// Demonstracja: Kontenery asocjacyjne
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o associative main.cpp && ./associative
// ============================================================

// Własna specjalizacja std::hash dla pary int
struct Punkt {
    int x, y;
    bool operator==(const Punkt& o) const { return x == o.x && y == o.y; }
};

namespace std {
    template<>
    struct hash<Punkt> {
        size_t operator()(const Punkt& p) const noexcept {
            size_t h1 = std::hash<int>{}(p.x);
            size_t h2 = std::hash<int>{}(p.y);
            return h1 ^ (h2 << 1);
        }
    };
}

void demo_map() {
    std::cout << "=== std::map ===\n";
    std::map<std::string, int> oceny;

    oceny["Anna"]   = 5;
    oceny["Bartek"] = 4;
    oceny.emplace("Celina", 5);

    // Iteracja w kolejności alfabetycznej
    for (const auto& [imie, ocena] : oceny)
        std::cout << imie << ": " << ocena << "\n";

    // insert – sprawdza czy wstawiono
    auto [it, ok] = oceny.insert({"Anna", 3});  // nie nadpisze!
    std::cout << "Anna wstawiona ponownie: " << std::boolalpha << ok << "\n";

    // try_emplace (C++17)
    oceny.try_emplace("Darek", 3);
    oceny.try_emplace("Anna", 1);  // nie nadpisze
    std::cout << "Anna po try_emplace: " << oceny["Anna"] << "\n";

    // find
    if (auto f = oceny.find("Bartek"); f != oceny.end())
        std::cout << "Znaleziono: " << f->second << "\n";

    // contains (C++20)
    std::cout << "Czy Celina: " << oceny.contains("Celina") << "\n";
}

void demo_set() {
    std::cout << "\n=== std::set ===\n";
    std::vector<int> v = {5, 3, 1, 4, 1, 5, 9, 2, 6, 5};

    // Szybkie usunięcie duplikatów
    std::set<int> s(v.begin(), v.end());
    std::cout << "Unikalne: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // lower_bound / upper_bound
    auto lo = s.lower_bound(3);  // >= 3
    auto hi = s.upper_bound(6);  // > 6
    std::cout << "Zakres [3,6]: ";
    for (auto it = lo; it != hi; ++it) std::cout << *it << " ";
    std::cout << "\n";
}

void demo_unordered_map() {
    std::cout << "\n=== std::unordered_map ===\n";

    std::string tekst = "to jest prosty tekst z kilkoma slowami i slowem to";
    std::unordered_map<std::string, int> freq;

    // Zliczanie częstotliwości słów
    std::istringstream iss(tekst);
    // ręcznie przez stringstream
    std::string slowo;
    // Prostsza wersja bez sstream:
    for (char& c : tekst) if (c == ' ') c = '\n';
    std::string buf;
    for (char c : tekst) {
        if (c == '\n') {
            if (!buf.empty()) { ++freq[buf]; buf.clear(); }
        } else {
            buf += c;
        }
    }
    if (!buf.empty()) ++freq[buf];

    for (const auto& [w, n] : freq)
        if (n > 1) std::cout << w << ": " << n << "x\n";

    std::cout << "bucket_count: " << freq.bucket_count() << "\n";
    std::cout << "load_factor:  " << freq.load_factor()  << "\n";
}

void demo_custom_hash() {
    std::cout << "\n=== unordered_map z własnym hashem ===\n";
    std::unordered_map<Punkt, std::string> mapa;
    mapa[{0, 0}] = "Poczatek";
    mapa[{3, 4}] = "Odleglosc=5";
    mapa[{1, 1}] = "Przekatna";

    for (const auto& [p, opis] : mapa)
        std::cout << "(" << p.x << "," << p.y << ") → " << opis << "\n";
}

int main() {
    demo_map();
    demo_set();
    demo_unordered_map();
    demo_custom_hash();
    return 0;
}
