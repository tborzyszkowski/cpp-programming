#include <iostream>
#include <concepts>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <ranges>
#include <numeric>

// ============================================================
// Demonstracja: C++20 Concepts
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o concepts main.cpp && ./concepts
// ============================================================

// ============================================================
// 1. Definiowanie własnych concepts
// ============================================================

// Prosty concept jako alias na type_trait
template<typename T>
concept Calkowity = std::is_integral_v<T>;

// Concept z requires expression
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// Concept sprawdzający API kontenera
template<typename T>
concept Kontener = requires(T c) {
    c.begin();
    c.end();
    c.size();
    typename T::value_type;
};

// Concept z zagnieżdżonym requires
template<typename T>
concept Serializowalny = requires(T x) {
    { x.to_string() } -> std::convertible_to<std::string>;
};

void demo_definiowanie() {
    std::cout << "=== Własne Concepts ===\n";

    std::cout << std::boolalpha;
    std::cout << "Calkowity<int>:    " << Calkowity<int>    << "\n";
    std::cout << "Calkowity<double>: " << Calkowity<double> << "\n";
    std::cout << "Addable<int>:      " << Addable<int>      << "\n";
    std::cout << "Addable<std::string>: " << Addable<std::string> << "\n";
    std::cout << "Kontener<std::vector<int>>: " << Kontener<std::vector<int>> << "\n";
    std::cout << "Kontener<int>:     " << Kontener<int>     << "\n";
}

// ============================================================
// 2. Cztery sposoby użycia concepts w szablonach
// ============================================================

// Sposób 1: requires po template<>
template<typename T>
    requires std::integral<T>
T sposob1(T a, T b) { return a + b; }

// Sposób 2: requires po sygnaturze
template<typename T>
T sposob2(T a, T b) requires std::integral<T> { return a + b; }

// Sposób 3: concept zamiast typename
template<std::integral T>
T sposob3(T a, T b) { return a + b; }

// Sposób 4: abbreviated function template
auto sposob4(std::integral auto a, std::integral auto b) { return a + b; }

void demo_sposoby_uzycia() {
    std::cout << "\n=== Cztery sposoby użycia Concepts ===\n";
    std::cout << "sposob1(3,4) = " << sposob1(3, 4) << "\n";
    std::cout << "sposob2(3,4) = " << sposob2(3, 4) << "\n";
    std::cout << "sposob3(3,4) = " << sposob3(3, 4) << "\n";
    std::cout << "sposob4(3,4) = " << sposob4(3, 4) << "\n";
    // Wszystkie odrzucą double – test (odkomentuj by zobaczyć błąd):
    // sposob4(1.5, 2.5);  // BŁĄD: double nie spełnia integral
}

// ============================================================
// 3. Wbudowane concepts ze standardu
// ============================================================
template<std::integral T>
void f_integral(T x) { std::cout << "integral: " << x << "\n"; }

template<std::unsigned_integral T>
void f_integral(T x) { std::cout << "unsigned integral: " << x << "\n"; }

template<std::floating_point T>
void f_fp(T x) { std::cout << "floating_point: " << x << "\n"; }

template<std::regular T>
void f_regular(T x) { std::cout << "regular: " << x << "\n"; }

void demo_wbudowane() {
    std::cout << "\n=== Wbudowane Concepts ===\n";

    // Subsumpcja: unsigned_integral jest bardziej szczegółowy niż integral
    f_integral(42);    // signed   → "integral"
    f_integral(42u);   // unsigned → "unsigned integral" (subsumpcja!)

    f_fp(3.14);
    f_fp(2.71828f);

    f_regular(std::string{"hello"});  // string jest regular
}

// ============================================================
// 4. Concepts z ranges
// ============================================================
template<std::ranges::input_range R>
void drukuj_zakres(const R& r, std::string_view nazwa) {
    std::cout << nazwa << ": ";
    for (const auto& x : r) std::cout << x << " ";
    std::cout << "\n";
}

template<std::ranges::random_access_range R>
void sortuj_i_drukuj(R r) {  // kopia
    std::ranges::sort(r);
    std::cout << "sorted: ";
    drukuj_zakres(r, "");
}

void demo_ranges() {
    std::cout << "\n=== Concepts z Ranges ===\n";

    std::vector<int> v{5, 3, 1, 4, 2};
    std::list<int>   l{5, 3, 1, 4, 2};

    drukuj_zakres(v, "vector");
    drukuj_zakres(l, "list");   // list jest input_range – OK

    sortuj_i_drukuj(v);         // vector jest random_access – OK
    // sortuj_i_drukuj(l);      // list nie jest random_access – BŁĄD kompilacji

    // Ranges views pipeline
    auto wynik = v
        | std::views::filter([](int x) { return x % 2 != 0; })
        | std::views::transform([](int x) { return x * x; });

    std::cout << "nieparzyste^2: ";
    for (int x : wynik) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 5. Concept z wymaganiami złożonymi (compound requirements)
// ============================================================
template<typename T>
concept Porównywalny = requires(T a, T b) {
    { a < b }  -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

template<Porównywalny T>
T min_element_val(std::vector<T>& v) {
    return *std::min_element(v.begin(), v.end());
}

void demo_zlożone() {
    std::cout << "\n=== Złożone wymagania w Concept ===\n";

    std::vector<int>         vi{5, 3, 8, 1, 9};
    std::vector<std::string> vs{"banana", "apple", "cherry"};

    std::cout << "min int: "    << min_element_val(vi) << "\n";
    std::cout << "min string: " << min_element_val(vs) << "\n";
    // min_element_val(std::vector<std::pair<int,int>>{{1,2}});
    // BŁĄD – pair<int,int> nie spełnia Porównywalny bez operator<
}

int main() {
    demo_definiowanie();
    demo_sposoby_uzycia();
    demo_wbudowane();
    demo_ranges();
    demo_zlożone();
    return 0;
}
