#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include <memory>
#include <type_traits>

// ============================================================
// Demonstracja: Zaawansowane cechy lambd
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o advanced main.cpp && ./advanced
// ============================================================

// ============================================================
// 1. Lambdy generyczne (C++14)
// ============================================================
void demo_generyczne() {
    std::cout << "=== Lambdy generyczne (C++14) ===\n";

    // auto parametry → szablonowy operator()
    auto dodaj = [](auto a, auto b) { return a + b; };
    std::cout << "int:    " << dodaj(1, 2) << "\n";
    std::cout << "double: " << dodaj(1.5, 2.5) << "\n";
    std::cout << "string: " << dodaj(std::string{"Hello "}, std::string{"World"}) << "\n";

    // Przydatne w algorytmach STL
    std::vector<std::pair<int, std::string>> v = {{3,"c"},{1,"a"},{2,"b"}};
    std::sort(v.begin(), v.end(),
              [](const auto& x, const auto& y){ return x.first < y.first; });
    std::cout << "Po sort: ";
    for (auto& [n, s] : v) std::cout << n << ":" << s << " ";
    std::cout << "\n";

    // Lambda generyczna jako "perfect forwarder"
    auto drukuj = [](auto&&... args){
        ((std::cout << args << " "), ...);
        std::cout << "\n";
    };
    drukuj(1, 2.5, "hello", 'x');
}

// ============================================================
// 2. Lambdy szablonowe (C++20)
// ============================================================
void demo_szablonowe() {
    std::cout << "\n=== Lambdy szablonowe (C++20) ===\n";

    // Jawna lista parametrów szablonu
    auto suma_v = []<typename T>(const std::vector<T>& v) -> T {
        return std::accumulate(v.begin(), v.end(), T{});
    };

    std::vector<int>    vi = {1, 2, 3, 4, 5};
    std::vector<double> vd = {1.1, 2.2, 3.3};
    std::cout << "Suma int:    " << suma_v(vi) << "\n";
    std::cout << "Suma double: " << suma_v(vd) << "\n";

    // Wymuszenie tego samego typu dla obu argumentów
    auto max_same_type = []<typename T>(T a, T b) { return a > b ? a : b; };
    std::cout << "Max(3, 7): " << max_same_type(3, 7) << "\n";
    std::cout << "Max(3.5, 2.1): " << max_same_type(3.5, 2.1) << "\n";
    // max_same_type(1, 2.0);  // BŁĄD: T musi być jednym typem

    // Z concepts (C++20)
    auto suma_int = []<std::integral T>(T a, T b) -> T { return a + b; };
    std::cout << "suma_int(3, 4) = " << suma_int(3, 4) << "\n";
    // suma_int(1.5, 2.5);  // BŁĄD: double nie spełnia std::integral
}

// ============================================================
// 3. Rekurencyjna lambda
// ============================================================
void demo_rekurencja() {
    std::cout << "\n=== Rekurencyjna lambda ===\n";

    // Sposób 1: przez std::function (z overhead'em)
    std::function<long long(int)> silnia = [&silnia](int n) -> long long {
        return n <= 1 ? 1 : n * silnia(n - 1);
    };
    std::cout << "10! = " << silnia(10) << "\n";

    // Sposób 2: self-reference przez auto (zero overhead, C++14)
    auto fib = [](auto self, int n) -> int {
        if (n <= 1) return n;
        return self(self, n - 1) + self(self, n - 2);
    };
    std::cout << "fib(10) = " << fib(fib, 10) << "\n";

    // Rekurencja w obchodzie drzewa
    struct Wezel {
        int wartosc;
        std::vector<Wezel> dzieci;
    };

    std::function<int(const Wezel&)> suma_drzewa = [&](const Wezel& w) -> int {
        int s = w.wartosc;
        for (const auto& d : w.dzieci) s += suma_drzewa(d);
        return s;
    };

    Wezel drzewo{1, {{2, {{4,{}},{5,{}}}},{3,{{6,{}}}}}};
    std::cout << "Suma drzewa (1+2+3+4+5+6) = " << suma_drzewa(drzewo) << "\n";
}

// ============================================================
// 4. IIFE – Immediately Invoked Function Expression
// ============================================================
void demo_iife() {
    std::cout << "\n=== IIFE ===\n";

    // Inicjalizacja stałej z kompleksową logiką
    const int suma_do_100 = []{
        int s = 0;
        for (int i = 1; i <= 100; ++i) s += i;
        return s;
    }();
    std::cout << "Suma 1..100 = " << suma_do_100 << "\n";

    // Inicjalizacja zależna od warunku (zamiast ?: dla złożonej logiki)
    int kod = 404;
    const std::string opis = [kod]() -> std::string {
        switch (kod) {
            case 200: return "OK";
            case 201: return "Created";
            case 400: return "Bad Request";
            case 404: return "Not Found";
            case 500: return "Internal Server Error";
            default:  return "Unknown (" + std::to_string(kod) + ")";
        }
    }();
    std::cout << "HTTP " << kod << " → " << opis << "\n";

    // IIFE z przechwyceniem
    std::vector<int> v = {5, 3, 1, 4, 2};
    const bool jest_posortowany = [&v]{
        for (std::size_t i = 1; i < v.size(); ++i)
            if (v[i] < v[i-1]) return false;
        return true;
    }();
    std::cout << "Wektor posortowany: " << (jest_posortowany ? "tak" : "nie") << "\n";
}

// ============================================================
// 5. Kompozycja lambd
// ============================================================
void demo_kompozycja() {
    std::cout << "\n=== Kompozycja lambd ===\n";

    // compose(f, g)(x) = f(g(x))
    auto compose = [](auto f, auto g) {
        return [f, g](auto x){ return f(g(x)); };
    };

    auto podwoj  = [](int x){ return x * 2; };
    auto dodaj10 = [](int x){ return x + 10; };
    auto kw      = [](int x){ return x * x; };

    auto f1 = compose(dodaj10, podwoj);  // g: *2, f: +10
    auto f2 = compose(kw, dodaj10);      // g: +10, f: ^2
    auto f3 = compose(kw, compose(dodaj10, podwoj));

    std::cout << "compose(dodaj10, podwoj)(5) = " << f1(5) << "\n";   // (5*2)+10=20
    std::cout << "compose(kw, dodaj10)(3)     = " << f2(3) << "\n";   // (3+10)^2=169
    std::cout << "compose(kw,compose(...))(3) = " << f3(3) << "\n";   // ((3*2)+10)^2=256

    // Pipeline (stosuje po kolei)
    auto pipeline = [](auto... funcs) {
        return [funcs...](auto x){
            return ((x = funcs(x)), ...);  // fold expression C++17
        };
    };

    auto proc = pipeline(podwoj, dodaj10, kw);
    std::cout << "pipeline(podwoj,dodaj10,kw)(3) = " << proc(3) << "\n";  // ((3*2)+10)^2=256
}

// ============================================================
// 6. std::function vs auto – wydajność
// ============================================================
void demo_wydajnosc() {
    std::cout << "\n=== std::function vs auto ===\n";

    auto lambda = [](int x){ return x * x; };

    // Sposób 1: auto – zerowy overhead, inline możliwy
    auto f_auto = lambda;
    std::cout << "auto: " << f_auto(5) << "\n";

    // Sposób 2: std::function – type erasure, virtual call
    std::function<int(int)> f_sf = lambda;
    std::cout << "std::function: " << f_sf(5) << "\n";

    // Sposób 3: template – najlepszy dla bibliotek
    auto zastosuj = [](auto func, int x){ return func(x); };
    std::cout << "template:      " << zastosuj(lambda, 5) << "\n";

    // std::function przydatny gdy:
    // 1. Trzeba przechować różne lambdy w kontenerze
    std::vector<std::function<int(int)>> ops;
    ops.push_back([](int x){ return x + 1; });
    ops.push_back([](int x){ return x * 2; });
    ops.push_back([](int x){ return x - 3; });

    int v = 10;
    for (auto& op : ops) v = op(v);
    std::cout << "Pipeline przez wektor std::function: " << v << "\n"; // (10+1)*2-3=19
}

int main() {
    demo_generyczne();
    demo_szablonowe();
    demo_rekurencja();
    demo_iife();
    demo_kompozycja();
    demo_wydajnosc();
    return 0;
}
