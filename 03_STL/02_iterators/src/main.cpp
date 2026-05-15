#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <array>
#include <iterator>
#include <algorithm>
#include <ranges>

// ============================================================
// Demonstracja: Iteratory w C++
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o iterators main.cpp && ./iterators
// ============================================================

void demo_podstawy() {
    std::cout << "=== Podstawy iteratorów ===\n";
    std::vector<int> v = {10, 20, 30, 40, 50};

    // Ręczna iteracja
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    // std::next / std::prev / std::advance
    auto it = v.begin();
    std::advance(it, 2);
    std::cout << "Po advance(2): " << *it << "\n";         // 30
    std::cout << "next(it,1):    " << *std::next(it, 1) << "\n"; // 40
    std::cout << "prev(it,1):    " << *std::prev(it, 1) << "\n"; // 20
    std::cout << "distance:      " << std::distance(v.begin(), v.end()) << "\n"; // 5
}

void demo_kategorie() {
    std::cout << "\n=== Kategorie iteratorów ===\n";

    // RandomAccess (vector) – skok o n
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto vit = vec.begin();
    vit += 3;
    std::cout << "vector[3] przez iterator: " << *vit << "\n"; // 4

    // Bidirectional (list) – tylko ++ i --
    std::list<int> lst = {1, 2, 3, 4, 5};
    auto lit = lst.end();
    --lit;
    std::cout << "list last:  " << *lit << "\n"; // 5
    --lit;
    std::cout << "list prev:  " << *lit << "\n"; // 4

    // Forward (forward_list) – tylko ++
    std::forward_list<int> fl = {1, 2, 3};
    for (auto fit = fl.begin(); fit != fl.end(); ++fit)
        std::cout << *fit << " ";
    std::cout << "\n";
}

void demo_odwrotne() {
    std::cout << "\n=== Iteratory odwrotne ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5};

    for (auto rit = v.rbegin(); rit != v.rend(); ++rit)
        std::cout << *rit << " ";  // 5 4 3 2 1
    std::cout << "\n";
}

void demo_const_iterator() {
    std::cout << "\n=== const_iterator ===\n";
    const std::vector<int> cv = {10, 20, 30};
    for (auto it = cv.cbegin(); it != cv.cend(); ++it)
        std::cout << *it << " ";  // kompilator wymusi brak modyfikacji
    std::cout << "\n";
}

void demo_uniewnaznienie() {
    std::cout << "\n=== Unieważnienie iteratora po push_back ===\n";
    std::vector<int> v = {1, 2, 3};
    v.reserve(10);               // rezerwujemy miejsce – push_back NIE realokuje
    auto it = v.begin();
    v.push_back(4);
    // Bezpieczne tylko dlatego, że reserve() zabezpieczyło przed realokacją
    std::cout << "Pierwszy element: " << *it << "\n";
}

void demo_ranges() {
    std::cout << "\n=== C++20 Ranges ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // Stary styl
    std::sort(v.begin(), v.end());

    // Nowy styl
    std::ranges::sort(v);

    // Pipeline: parzyste → kwadraty
    auto pipeline = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });

    std::cout << "Kwadraty parzystych: ";
    for (int x : pipeline) std::cout << x << " ";  // 4 16 36 64
    std::cout << "\n";

    // views::take, views::drop
    auto pierwsze_trzy = v | std::views::take(3);
    std::cout << "Pierwsze 3: ";
    for (int x : pierwsze_trzy) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    demo_podstawy();
    demo_kategorie();
    demo_odwrotne();
    demo_const_iterator();
    demo_uniewnaznienie();
    demo_ranges();
    return 0;
}
