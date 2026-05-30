#include <iostream>
#include <functional>
#include <any>
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <typeinfo>
#include <algorithm>
#include <numeric>
#include <cmath>

// ============================================================
// Demonstracja: Type Erasure w C++
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o type_erasure main.cpp && ./type_erasure
// ============================================================

// ============================================================
// 1. Type erasure przez wirtualne funkcje
// ============================================================
struct IFigura {
    virtual double pole()  const = 0;
    virtual void   rysuj() const = 0;
    virtual ~IFigura() = default;
};

struct Kolo : IFigura {
    double r;
    explicit Kolo(double r) : r(r) {}
    double pole()  const override { return 3.14159 * r * r; }
    void   rysuj() const override { std::cout << "O(r=" << r << ") "; }
};

struct Kwadrat : IFigura {
    double a;
    explicit Kwadrat(double a) : a(a) {}
    double pole()  const override { return a * a; }
    void   rysuj() const override { std::cout << "[" << a << "x" << a << "] "; }
};

void demo_virtual() {
    std::cout << "=== Type erasure: virtual dispatch ===\n";

    std::vector<std::unique_ptr<IFigura>> figury;
    figury.push_back(std::make_unique<Kolo>(5.0));
    figury.push_back(std::make_unique<Kwadrat>(3.0));
    figury.push_back(std::make_unique<Kolo>(2.0));

    std::cout << "Figury: ";
    for (const auto& f : figury) f->rysuj();
    std::cout << "\n";

    double suma = 0;
    for (const auto& f : figury) suma += f->pole();
    std::cout << "Suma pól: " << suma << "\n";
}

// ============================================================
// 2. std::function – type erasure dla callable
// ============================================================
void demo_std_function() {
    std::cout << "\n=== std::function type erasure ===\n";

    std::vector<std::function<int(int)>> transformacje;

    // Lambda
    transformacje.push_back([](int x) { return x * 2; });
    // Lambda z domknięciem
    int offset = 100;
    transformacje.push_back([offset](int x) { return x + offset; });
    // Funktor
    struct Kwadrat { int operator()(int x) const { return x * x; } };
    transformacje.push_back(Kwadrat{});

    for (int i = 0; i < (int)transformacje.size(); ++i)
        std::cout << "transformacja[" << i << "](5) = " << transformacje[i](5) << "\n";

    // System zdarzeń oparty na std::function
    std::map<std::string, std::vector<std::function<void(const std::string&)>>> handlers;
    handlers["click"].push_back([](const std::string& s) {
        std::cout << "  [LOG] klik: " << s << "\n";
    });
    handlers["click"].push_back([](const std::string& s) {
        std::cout << "  [UI] refresh after: " << s << "\n";
    });

    std::cout << "Zdarzenie 'click':\n";
    for (auto& h : handlers["click"]) h("button1");
}

// ============================================================
// 3. std::any – type erasure dla dowolnego typu
// ============================================================
void demo_std_any() {
    std::cout << "\n=== std::any type erasure ===\n";

    std::any x;

    x = 42;
    std::cout << "int: " << std::any_cast<int>(x) << "\n";

    x = std::string{"hello"};
    std::cout << "string: " << std::any_cast<std::string>(x) << "\n";

    x = std::vector<int>{1, 2, 3};
    if (x.type() == typeid(std::vector<int>)) {
        auto& v = std::any_cast<std::vector<int>&>(x);
        std::cout << "vector size: " << v.size() << "\n";
    }

    // Mapa konfiguracji z różnymi typami wartości
    std::map<std::string, std::any> config;
    config["host"]    = std::string{"localhost"};
    config["port"]    = 8080;
    config["debug"]   = true;
    config["timeout"] = 30.5;

    std::cout << "host: "    << std::any_cast<std::string>(config["host"])  << "\n";
    std::cout << "port: "    << std::any_cast<int>(config["port"])          << "\n";
    std::cout << "debug: "   << std::boolalpha
                              << std::any_cast<bool>(config["debug"])        << "\n";
    std::cout << "timeout: " << std::any_cast<double>(config["timeout"])    << "\n";

    // Złe rzutowanie
    try {
        std::any_cast<int>(config["host"]);   // host to string, nie int
    } catch (const std::bad_any_cast& e) {
        std::cout << "bad_any_cast: " << e.what() << "\n";
    }
}

// ============================================================
// 4. std::variant – type erasure zamknięty
// ============================================================
using Wartosc = std::variant<int, double, std::string, bool>;

// Overload pattern – wygodne tworzenie visitora
template<typename... Fs>
struct Overloaded : Fs... { using Fs::operator()...; };
template<typename... Fs>
Overloaded(Fs...) -> Overloaded<Fs...>;

void demo_std_variant() {
    std::cout << "\n=== std::variant type erasure ===\n";

    std::vector<Wartosc> wartosci = {
        42, 3.14, std::string{"C++"}, true, -7, std::string{"world"}
    };

    auto drukuj = Overloaded{
        [](int x)               { std::cout << "int(" << x << ") "; },
        [](double x)            { std::cout << "dbl(" << x << ") "; },
        [](const std::string& s){ std::cout << "str(" << s << ") "; },
        [](bool b)              { std::cout << "bool(" << b << ") "; },
    };

    for (const auto& w : wartosci) std::visit(drukuj, w);
    std::cout << "\n";

    // Przykład: kalkulator z wariantami
    auto oblicz = [](Wartosc a, Wartosc b) -> Wartosc {
        return std::visit([](auto x, auto y) -> Wartosc {
            if constexpr (std::is_arithmetic_v<decltype(x)> &&
                          std::is_arithmetic_v<decltype(y)>)
                return x + y;
            else
                return std::string{"??"};
        }, a, b);
    };

    auto wynik = oblicz(Wartosc{10}, Wartosc{3.14});
    std::visit([](const auto& v) { std::cout << "10 + 3.14 = " << v << "\n"; }, wynik);
}

// ============================================================
// 5. Własne type erasure – wartościowa semantyka bez dziedziczenia w API
// ============================================================
class Figura {
    struct Concept {
        virtual double pole()  const = 0;
        virtual void   rysuj() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
        virtual ~Concept() = default;
    };

    template<typename T>
    struct Model final : Concept {
        T obj_;
        explicit Model(T x) : obj_(std::move(x)) {}
        double pole()  const override { return obj_.pole(); }
        void   rysuj() const override { obj_.rysuj(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<T>>(obj_);
        }
    };

    std::unique_ptr<Concept> impl_;

public:
    template<typename T>
    Figura(T x) : impl_(std::make_unique<Model<T>>(std::move(x))) {}

    Figura(const Figura& o) : impl_(o.impl_->clone()) {}
    Figura(Figura&&) = default;
    Figura& operator=(Figura o) { std::swap(impl_, o.impl_); return *this; }

    double pole()  const { return impl_->pole(); }
    void   rysuj() const { impl_->rysuj(); }
};

struct TrojkatFig {
    double a, b, c;
    double pole() const {
        double s = (a + b + c) / 2;
        return std::sqrt(s * (s-a) * (s-b) * (s-c));
    }
    void rysuj() const { std::cout << "△(" << a << "," << b << "," << c << ") "; }
};

void demo_wlasne_te() {
    std::cout << "\n=== Własne Type Erasure (wartościowa semantyka) ===\n";

    // Figura przyjmuje DOWOLNY typ z pole()/rysuj() – bez dziedziczenia!
    std::vector<Figura> figury;
    figury.emplace_back(Kolo{3.0});
    figury.emplace_back(Kwadrat{4.0});
    figury.emplace_back(TrojkatFig{3.0, 4.0, 5.0});

    std::cout << "Rysuj: ";
    for (const auto& f : figury) f.rysuj();
    std::cout << "\n";

    double suma = 0;
    for (const auto& f : figury) suma += f.pole();
    std::cout << "Suma pól: " << suma << "\n";

    // Głęboka kopia – wartościowa semantyka!
    Figura kopia = figury[0];
    std::cout << "Kopia pola[0]: " << kopia.pole() << "\n";
}

int main() {
    demo_virtual();
    demo_std_function();
    demo_std_any();
    demo_std_variant();
    demo_wlasne_te();
    return 0;
}
