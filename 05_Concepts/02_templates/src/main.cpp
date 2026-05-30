#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <concepts>
#include <numeric>

// ============================================================
// Demonstracja: Szablony C++ – składnia i mechanizmy
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o templates main.cpp && ./templates
// ============================================================

// ============================================================
// 1. Szablony funkcji
// ============================================================
template<typename T>
T maks(T a, T b) { return a > b ? a : b; }

template<typename T, typename U>
auto dodaj(T a, U b) { return a + b; }

void demo_szablony_funkcji() {
    std::cout << "=== Szablony funkcji ===\n";
    std::cout << "maks(3,7) = "    << maks(3, 7)        << "\n";
    std::cout << "maks(1.5,2.5) = " << maks(1.5, 2.5)   << "\n";
    std::cout << "maks(\"abc\",\"xyz\") = "
              << maks(std::string{"abc"}, std::string{"xyz"}) << "\n";
    std::cout << "dodaj(3, 1.5) = " << dodaj(3, 1.5)     << "\n";
}

// ============================================================
// 2. Szablony klas + specjalizacja
// ============================================================
template<typename T, std::size_t N = 8>
class TablicaStatyczna {
    T dane_[N];
    int rozmiar_ = 0;
public:
    void push(const T& x) {
        if (rozmiar_ < (int)N) dane_[rozmiar_++] = x;
    }
    T& operator[](int i) { return dane_[i]; }
    int rozmiar() const { return rozmiar_; }
};

// Pełna specjalizacja: bool przechowywany jako pojedynczy bit w int
template<>
class TablicaStatyczna<bool, 32> {
    unsigned int maska_ = 0;
    int          rozmiar_ = 0;
public:
    void push(bool x) {
        if (rozmiar_ < 32) {
            if (x) maska_ |= (1u << rozmiar_);
            ++rozmiar_;
        }
    }
    bool operator[](int i) const { return (maska_ >> i) & 1u; }
    int  rozmiar() const { return rozmiar_; }
};

void demo_szablony_klas() {
    std::cout << "\n=== Szablony klas ===\n";

    TablicaStatyczna<int, 4> ti;
    ti.push(10); ti.push(20); ti.push(30);
    std::cout << "TablicaStatyczna<int,4>[1] = " << ti[1] << "\n";

    TablicaStatyczna<bool, 32> tb;
    tb.push(true); tb.push(false); tb.push(true);
    std::cout << "TablicaStatyczna<bool,32>[0] = " << tb[0] << "\n";
    std::cout << "TablicaStatyczna<bool,32>[1] = " << tb[1] << "\n";

    // C++17 CTAD
    std::vector v{1, 2, 3, 4, 5};   // vector<int> dedukcja
    std::cout << "CTAD vector[2] = " << v[2] << "\n";
}

// ============================================================
// 3. SFINAE + enable_if
// ============================================================
template<typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
opisz(T x) { return "calkowity(" + std::to_string(x) + ")"; }

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::string>
opisz(T x) { return "zmiennoprzecinkowy(" + std::to_string(x) + ")"; }

// if constexpr – prostsza alternatywa dla SFINAE
template<typename T>
std::string opisz_if(T x) {
    if constexpr (std::is_integral_v<T>)
        return "calkowity(" + std::to_string(x) + ")";
    else if constexpr (std::is_floating_point_v<T>)
        return "zmiennoprzecinkowy(" + std::to_string(x) + ")";
    else
        return "inny";
}

void demo_sfinae() {
    std::cout << "\n=== SFINAE + if constexpr ===\n";
    std::cout << opisz(42)    << "\n";
    std::cout << opisz(3.14)  << "\n";
    std::cout << opisz_if(7)  << "\n";
    std::cout << opisz_if(2.71828) << "\n";
}

// ============================================================
// 4. Variadic templates + fold expressions
// ============================================================
template<typename... Ts>
auto suma(Ts... args) { return (... + args); }

template<typename... Ts>
void drukuj_wszystko(Ts&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

template<typename... Ts>
bool wszystkie_dodatnie(Ts... args) {
    return ((args > 0) && ...);
}

void demo_variadic() {
    std::cout << "\n=== Variadic templates + fold expressions ===\n";
    std::cout << "suma(1,2,3,4) = " << suma(1, 2, 3, 4) << "\n";
    std::cout << "suma(1.1, 2.2, 3.3) = " << suma(1.1, 2.2, 3.3) << "\n";

    drukuj_wszystko("Hello", 42, 3.14, true);

    std::cout << "wszystkie_dodatnie(1,2,3) = "
              << std::boolalpha << wszystkie_dodatnie(1, 2, 3) << "\n";
    std::cout << "wszystkie_dodatnie(1,-2,3) = "
              << wszystkie_dodatnie(1, -2, 3) << "\n";
}

// ============================================================
// 5. Type traits
// ============================================================
void demo_type_traits() {
    std::cout << "\n=== Type traits ===\n";

    // Pytania o typy
    std::cout << std::boolalpha;
    std::cout << "is_integral<int>:      " << std::is_integral_v<int>       << "\n";
    std::cout << "is_integral<double>:   " << std::is_integral_v<double>    << "\n";
    std::cout << "is_same<int,int>:      " << std::is_same_v<int, int>      << "\n";
    std::cout << "is_same<int,long>:     " << std::is_same_v<int, long>     << "\n";
    std::cout << "is_pointer<int*>:      " << std::is_pointer_v<int*>       << "\n";
    std::cout << "is_pointer<int>:       " << std::is_pointer_v<int>        << "\n";

    // Transformacje typów
    using ConstInt  = std::add_const_t<int>;
    using PlainInt  = std::remove_const_t<ConstInt>;
    static_assert(std::is_same_v<PlainInt, int>);
    std::cout << "remove_const_t<const int> == int: " << std::is_same_v<PlainInt, int> << "\n";
}

// ============================================================
// 6. C++20: requires w szablonie (przedsmak Concepts)
// ============================================================
template<typename T>
    requires std::is_arithmetic_v<T>
T kwadrat(T x) { return x * x; }

// Inline requires (bardziej zwięzłe)
template<typename T>
T szescian(T x) requires std::is_arithmetic_v<T> { return x * x * x; }

void demo_requires() {
    std::cout << "\n=== C++20 requires (przed Concepts) ===\n";
    std::cout << "kwadrat(5) = "     << kwadrat(5)      << "\n";
    std::cout << "kwadrat(2.5) = "   << kwadrat(2.5)    << "\n";
    std::cout << "szescian(3) = "    << szescian(3)     << "\n";
    // kwadrat("hello");  // BŁĄD KOMPILACJI – czytelny komunikat
}

int main() {
    demo_szablony_funkcji();
    demo_szablony_klas();
    demo_sfinae();
    demo_variadic();
    demo_type_traits();
    demo_requires();
    return 0;
}
