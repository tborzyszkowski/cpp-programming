#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <memory>
#include <numeric>

// ============================================================
// Demonstracja: Składnia i komponenty lambd
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o syntax main.cpp && ./syntax
// ============================================================

// ============================================================
// 1. Podstawowe formy składniowe
// ============================================================
void demo_podstawy() {
    std::cout << "=== Podstawowe formy ===\n";

    // Lambda minimalna (nie robi nic)
    auto nic = []{};
    nic();

    // Lambda bez parametrów, zwraca wartość
    auto pi = []{ return 3.14159; };
    std::cout << "pi ~ " << pi() << "\n";

    // Lambda z parametrami
    auto kwadrat = [](int x){ return x * x; };
    std::cout << "7^2 = " << kwadrat(7) << "\n";

    // Lambda z jawnym typem zwracanym
    auto dzielenie = [](int a, int b) -> double {
        return static_cast<double>(a) / b;
    };
    std::cout << "7/2 = " << dzielenie(7, 2) << "\n";

    // Lambda jako predykat
    auto jest_parzysta = [](int x) -> bool { return x % 2 == 0; };
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "Parzyste: ";
    for (int x : v)
        if (jest_parzysta(x)) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 2. Typ domknięcia i auto
// ============================================================
void demo_typ_domkniecia() {
    std::cout << "\n=== Typ domkniecia ===\n";

    auto f = [](int x){ return x * 2; };
    auto g = [](int x){ return x * 2; };  // inny typ!

    // typeid pokazuje anonimowe nazwy typów
    std::cout << "typ f: " << typeid(f).name() << "\n";
    std::cout << "typ g: " << typeid(g).name() << "\n";
    // (nazwy są nieczytelne – to właśnie "anonimowe")

    // Bezstanowa lambda → wskaźnik na funkcję
    int (*fp)(int) = [](int x){ return x * 3; };
    std::cout << "Przez wskaznik: " << fp(5) << "\n";

    // std::function – uniform type (z kosztem)
    std::function<int(int)> sf = [](int x){ return x + 100; };
    std::cout << "Przez std::function: " << sf(5) << "\n";

    // Szablon – najlepszy sposób przekazania lambdy (zero overhead)
    auto zastosuj = [](auto func, int x){ return func(x); };
    std::cout << "Przez szablon auto: " << zastosuj([](int x){ return x * 4; }, 5) << "\n";
}

// ============================================================
// 3. Dedukcja typu zwracanego
// ============================================================
void demo_dedukcja() {
    std::cout << "\n=== Dedukcja typu zwracanego ===\n";

    auto f_int    = [](int x){ return x * 2; };        // int
    auto f_double = [](double x){ return x * 2.0; };   // double
    auto f_bool   = [](int x){ return x > 0; };        // bool
    auto f_string = [](int n){ return std::string(n, '*'); }; // std::string

    std::cout << "int:    " << f_int(5)    << "\n";
    std::cout << "double: " << f_double(2.5) << "\n";
    std::cout << "bool:   " << f_bool(-3)  << "\n";
    std::cout << "string: " << f_string(5) << "\n";

    // Jawny typ – rozwiązuje konflikt lub wymusza konwersję
    auto f_explicit = [](int x) -> double {
        if (x > 0) return x;     // int → niejawna konwersja na double
        return -0.5;              // double
    };
    std::cout << "explicit -> double: " << f_explicit(3) << ", " << f_explicit(-1) << "\n";
}

// ============================================================
// 4. mutable
// ============================================================
void demo_mutable() {
    std::cout << "\n=== mutable ===\n";

    int x = 10;
    auto f_const  = [x](){ return x; };           // operator() const
    auto f_mutable = [x]() mutable {
        x += 5;                                    // modyfikuje lokalną KOPIĘ
        return x;
    };

    std::cout << "Przed: x = " << x << "\n";
    std::cout << "f_mutable(): " << f_mutable() << "\n";  // 15
    std::cout << "f_mutable(): " << f_mutable() << "\n";  // 20 – kopia się zwiększa
    std::cout << "Po: x = " << x << "\n";                  // 10 – oryginał niezmieniony

    // Klasyczny licznik z mutable + inicjalizator przechwycenia
    auto licznik = [n = 0]() mutable -> int { return ++n; };
    std::cout << "Licznik: " << licznik() << " "
                             << licznik() << " "
                             << licznik() << "\n";  // 1 2 3
}

// ============================================================
// 5. constexpr lambda (C++17)
// ============================================================
void demo_constexpr() {
    std::cout << "\n=== constexpr lambda (C++17) ===\n";

    constexpr auto silnia = [](int n) constexpr -> long long {
        long long wynik = 1;
        for (int i = 2; i <= n; ++i) wynik *= i;
        return wynik;
    };

    // Obliczenie w czasie kompilacji
    constexpr long long s10 = silnia(10);
    static_assert(s10 == 3628800, "Silnia 10 powinna byc 3628800");
    std::cout << "10! = " << s10 << " (czas kompilacji)\n";

    // I w czasie wykonania
    int n;
    std::cout << "Podaj n (dla n!): ";
    // Domyślna wartość dla demonstracji bez interakcji
    n = 7;
    std::cout << n << "\n";
    std::cout << n << "! = " << silnia(n) << "\n";

    constexpr auto potega2 = [](int exp) constexpr {
        int wynik = 1;
        for (int i = 0; i < exp; ++i) wynik <<= 1;
        return wynik;
    };
    static_assert(potega2(8) == 256);
    std::cout << "2^10 = " << potega2(10) << "\n";
}

// ============================================================
// 6. Inicjalizatory przechwycenia (C++14)
// ============================================================
void demo_init_capture() {
    std::cout << "\n=== Inicjalizatory przechwycenia (C++14) ===\n";

    int x = 10;

    // Przechwycenie z transformacją
    auto f = [y = x * 2 + 1](int n){ return n + y; };
    std::cout << "y = x*2+1 = " << (x*2+1) << ", f(5) = " << f(5) << "\n";

    // Przeniesienie (move) do domknięcia
    auto ptr = std::make_unique<int>(42);
    std::cout << "Przed move: ptr = " << (ptr ? "valid" : "null") << "\n";

    auto g = [p = std::move(ptr)](){
        return *p * 2;
    };
    std::cout << "Po move: ptr = " << (ptr ? "valid" : "null") << "\n";
    std::cout << "g() = " << g() << "\n";

    // Tworzenie złożonego stanu
    auto akumulator = [sum = 0, count = 0](int val) mutable -> double {
        sum += val;
        ++count;
        return count > 0 ? static_cast<double>(sum) / count : 0.0;
    };
    std::cout << "Srednia po 5: " << akumulator(5) << "\n";   // 5.0
    std::cout << "Srednia po 3: " << akumulator(3) << "\n";   // 4.0
    std::cout << "Srednia po 7: " << akumulator(7) << "\n";   // 5.0
}

int main() {
    demo_podstawy();
    demo_typ_domkniecia();
    demo_dedukcja();
    demo_mutable();
    demo_constexpr();
    demo_init_capture();
    return 0;
}
