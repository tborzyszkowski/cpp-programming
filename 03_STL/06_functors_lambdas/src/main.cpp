#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

// ============================================================
// Demonstracja: Funktory, lambdy, std::function
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o functors main.cpp && ./functors
// ============================================================

// --- Funktor z parametrem -----------------------------------
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

void demo_funktory() {
    std::cout << "=== Funktory ===\n";
    std::vector<int> v = {1, 3, 5, 7, 9, 11};

    std::cout << "Ile > 5: " << std::count_if(v.begin(), v.end(), WiekszyNiz{5}) << "\n";
    std::cout << "Ile > 8: " << std::count_if(v.begin(), v.end(), WiekszyNiz{8}) << "\n";

    std::vector<double> d = {1.0, 2.0, 3.0, 4.0};
    std::transform(d.begin(), d.end(), d.begin(), Mnoznik{2.5});
    std::cout << "Po x2.5: ";
    for (double x : d) std::cout << x << " ";
    std::cout << "\n";

    // Standardowe funktory
    std::sort(v.begin(), v.end(), std::greater<int>{});
    std::cout << "Malejąco: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

void demo_lambdy() {
    std::cout << "\n=== Lambdy ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Lambda z przechwyceniem przez wartość
    int prog = 5;
    auto wieksze = std::count_if(v.begin(), v.end(),
        [prog](int x){ return x > prog; });
    std::cout << "Ile > " << prog << ": " << wieksze << "\n";

    // Lambda modifikuje zmienną przez referencję
    int suma = 0;
    std::for_each(v.begin(), v.end(), [&suma](int x){ suma += x; });
    std::cout << "Suma: " << suma << "\n";

    // Lambda generyczna (C++14)
    auto pomnoz = [](auto a, auto b){ return a * b; };
    std::cout << "int: "    << pomnoz(3, 4)     << "\n";
    std::cout << "double: " << pomnoz(2.5, 4.0) << "\n";

    // Lambda mutable (własna kopia stanu)
    int licznik = 0;
    auto gen = [licznik]() mutable { return ++licznik; };
    std::cout << "gen: " << gen() << " " << gen() << " " << gen() << "\n";
    std::cout << "oryginał licznik: " << licznik << "\n";  // 0 – niezmieniony
}

void demo_std_function() {
    std::cout << "\n=== std::function ===\n";

    // Przechowywanie różnych callables w jednej zmiennej
    std::function<int(int)> f;

    f = [](int x){ return x * 2; };
    std::cout << "Lambda: " << f(5) << "\n";  // 10

    f = [](int x){ return x + 100; };
    std::cout << "Inna lambda: " << f(5) << "\n";  // 105

    // Wektor funkcji
    std::vector<std::function<int(int)>> pipeline;
    pipeline.push_back([](int x){ return x * 2; });
    pipeline.push_back([](int x){ return x + 10; });
    pipeline.push_back([](int x){ return x * x; });

    int wartosc = 3;
    for (auto& fn : pipeline) wartosc = fn(wartosc);
    std::cout << "Pipeline 3→ " << wartosc << "\n";  // (3*2+10)^2 = 256
}

void demo_bind() {
    std::cout << "\n=== std::bind vs lambda ===\n";
    using namespace std::placeholders;

    // bind
    auto dodaj10_bind = std::bind(std::plus<int>{}, _1, 10);
    std::cout << "bind: " << dodaj10_bind(5) << "\n";   // 15

    // Equivalent lambda – czytelniej
    auto dodaj10_lambda = [](int x){ return x + 10; };
    std::cout << "lambda: " << dodaj10_lambda(5) << "\n"; // 15

    // bind metody klasy
    struct Kalkulator {
        int baza;
        int oblicz(int x) const { return baza + x; }
    };
    Kalkulator k{100};
    auto met_bind = std::bind(&Kalkulator::oblicz, &k, _1);
    std::cout << "metoda bind: " << met_bind(42) << "\n";  // 142

    auto met_lambda = [&k](int x){ return k.oblicz(x); };
    std::cout << "metoda lambda: " << met_lambda(42) << "\n"; // 142
}

void demo_rekurencyjna_lambda() {
    std::cout << "\n=== Rekurencyjna lambda ===\n";
    std::function<int(int)> silnia = [&silnia](int n) -> int {
        return n <= 1 ? 1 : n * silnia(n - 1);
    };
    for (int i = 1; i <= 7; ++i)
        std::cout << i << "! = " << silnia(i) << "\n";
}

int main() {
    demo_funktory();
    demo_lambdy();
    demo_std_function();
    demo_bind();
    demo_rekurencyjna_lambda();
    return 0;
}
