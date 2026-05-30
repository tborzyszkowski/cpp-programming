#include <iostream>
#include <concepts>
#include <vector>
#include <string>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <cmath>

// ============================================================
// Demonstracja: Duck Typing w C++
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o duck_typing main.cpp && ./duck_typing
// ============================================================

// ============================================================
// 1. Niejawny duck typing – szablony bez concepts
// ============================================================
template<typename T>
std::size_t dlugosc(const T& x) {
    return x.size();   // niejawny kontrakt: T musi mieć size()
}

void demo_niejawny() {
    std::cout << "=== Niejawny duck typing (szablony) ===\n";
    std::cout << "dlugosc(vector{1,2,3}) = " << dlugosc(std::vector<int>{1,2,3}) << "\n";
    std::cout << "dlugosc(string{'abc'}) = "  << dlugosc(std::string{"abc"})      << "\n";
    std::cout << "dlugosc(\"hello\"s) = "     << dlugosc(std::string{"hello"})    << "\n";
    // dlugosc(42);  // BŁĄD kompilacji – int nie ma size()
}

// ============================================================
// 2. SFINAE duck typing – warunkowe przeciążenia (pre-C++20)
// ============================================================
template<typename T, typename = void>
struct MaSize : std::false_type {};

template<typename T>
struct MaSize<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
std::enable_if_t<MaSize<T>::value, std::size_t>
pobierz_rozmiar(const T& x) {
    return x.size();
}

template<typename T>
std::enable_if_t<!MaSize<T>::value, std::size_t>
pobierz_rozmiar(const T&) {
    return sizeof(T);   // fallback: rozmiar w bajtach
}

void demo_sfinae_duck() {
    std::cout << "\n=== SFINAE duck typing ===\n";
    std::cout << "pobierz_rozmiar(vector) = "
              << pobierz_rozmiar(std::vector<int>{1,2,3}) << "\n";
    std::cout << "pobierz_rozmiar(int) = "
              << pobierz_rozmiar(42) << " (sizeof)\n";
    std::cout << "pobierz_rozmiar(double) = "
              << pobierz_rozmiar(3.14) << " (sizeof)\n";
}

// ============================================================
// 3. Concepts – jawny duck typing z jasnym kontraktem
// ============================================================
template<typename T>
concept MaDzwiek = requires(T x) {
    { x.dzwiek() } -> std::convertible_to<std::string>;
};

template<typename T>
concept Ksztalt = requires(T k) {
    { k.pole()   } -> std::convertible_to<double>;
    { k.obwod()  } -> std::convertible_to<double>;
};

// Klasy BEZ dziedziczenia – "kaczki"
struct Kot  { std::string dzwiek() const { return "Miau"; } };
struct Pies { std::string dzwiek() const { return "Hau";  } };
struct Kolo {
    double r;
    double pole()  const { return 3.14159 * r * r; }
    double obwod() const { return 2 * 3.14159 * r; }
};
struct Prostokat {
    double a, b;
    double pole()  const { return a * b; }
    double obwod() const { return 2 * (a + b); }
};

// Funkcje przyjmujące "kaczki" przez concept
template<MaDzwiek T>
void glosno(const T& x) {
    std::cout << "Dźwięk: " << x.dzwiek() << "\n";
}

template<Ksztalt K>
void info_ksztaltu(const K& k) {
    std::cout << "Pole=" << k.pole()
              << " Obwód=" << k.obwod() << "\n";
}

void demo_concepts_duck() {
    std::cout << "\n=== Concepts duck typing ===\n";

    glosno(Kot{});     // Kot "kwacze" (ma dzwiek())
    glosno(Pies{});    // Pies też "kwacze"
    // glosno(Kolo{1.0});  // BŁĄD – Koło nie ma dzwiek()

    info_ksztaltu(Kolo{5.0});
    info_ksztaltu(Prostokat{3.0, 4.0});

    // Zewnętrzna klasa bez modyfikacji "staje się kształtem":
    struct TrojkatZewnetrzny {
        double a, b, c;
        double pole() const {
            double s = (a+b+c)/2;
            return std::sqrt(s*(s-a)*(s-b)*(s-c));
        }
        double obwod() const { return a + b + c; }
    };
    info_ksztaltu(TrojkatZewnetrzny{3.0, 4.0, 5.0});
}

// ============================================================
// 4. Porównanie: dziedziczenie vs duck typing
// ============================================================

// Styl z dziedziczeniem (nominatywny):
struct IFigura {
    virtual double pole()  const = 0;
    virtual double obwod() const = 0;
    virtual ~IFigura() = default;
};

struct KoloDerived : IFigura {
    double r;
    explicit KoloDerived(double r) : r(r) {}
    double pole()  const override { return 3.14159 * r * r; }
    double obwod() const override { return 2 * 3.14159 * r; }
};

// Styl z duck typing (strukturalny):
template<Ksztalt F>
double suma_pol(const std::vector<F>& ksztalty) {
    double suma = 0;
    for (const auto& k : ksztalty) suma += k.pole();
    return suma;
}

void demo_porownanie() {
    std::cout << "\n=== Dziedziczenie vs Duck Typing ===\n";

    // Dziedziczenie – polimorfizm runtime:
    std::vector<IFigura*> figury;
    KoloDerived k1{1.0}, k2{2.0};
    figury.push_back(&k1);
    figury.push_back(&k2);
    double suma_rt = 0;
    for (auto* f : figury) suma_rt += f->pole();
    std::cout << "Runtime (virtual): suma = " << suma_rt << "\n";

    // Duck typing – polimorfizm compile-time:
    std::vector<Kolo> kola{{1.0}, {2.0}};
    double suma_ct = suma_pol(kola);
    std::cout << "Compile-time (concept): suma = " << suma_ct << "\n";
}

// ============================================================
// 5. Callable duck typing – std::invocable
// ============================================================
template<std::invocable<int> F>
void zastosuj_do_10(F&& f) {
    std::cout << "f(10) = " << f(10) << "\n";
}

void demo_callable_duck() {
    std::cout << "\n=== Callable duck typing ===\n";

    // Lambda – "kwacze" jak callable(int)->int
    zastosuj_do_10([](int x) { return x * x; });
    // Funktor
    struct Podwoj { int operator()(int x) const { return x * 2; } };
    zastosuj_do_10(Podwoj{});
    // Funkcja globalna
    zastosuj_do_10([](int x) { return x + 100; });
}

int main() {
    demo_niejawny();
    demo_sfinae_duck();
    demo_concepts_duck();
    demo_porownanie();
    demo_callable_duck();
    return 0;
}
