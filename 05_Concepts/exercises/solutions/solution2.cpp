#include <iostream>
#include <concepts>
#include <variant>
#include <unordered_map>
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <typeindex>
#include <algorithm>
#include <sstream>

// ============================================================
// Rozwiązanie 2: TypeSafeRegistry z std::variant
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o solution2 solution2.cpp && ./solution2
// ============================================================

// ---- Typy wartości ----
using Wartosc = std::variant<int, double, std::string, bool>;

// ---- Wyjątki ----
struct RegistryKeyError : std::runtime_error {
    explicit RegistryKeyError(const std::string& klucz)
        : std::runtime_error("Klucz nie istnieje: '" + klucz + "'") {}
};

struct RegistryTypeError : std::runtime_error {
    RegistryTypeError(const std::string& klucz, const std::string& oczekiwany, const std::string& aktualny)
        : std::runtime_error("Klucz '" + klucz + "': oczekiwano " + oczekiwany + ", jest " + aktualny) {}
};

// Overload pattern do std::visit
template<typename... Fs>
struct Overloaded : Fs... { using Fs::operator()...; };
template<typename... Fs>
Overloaded(Fs...) -> Overloaded<Fs...>;

// ---- TypeSafeRegistry ----
class TypeSafeRegistry {
    std::unordered_map<std::string, Wartosc> dane_;

    static std::string nazwa_typu(const Wartosc& v) {
        return std::visit(Overloaded{
            [](int)               { return std::string{"int"}; },
            [](double)            { return std::string{"double"}; },
            [](const std::string&){ return std::string{"string"}; },
            [](bool)              { return std::string{"bool"}; },
        }, v);
    }

public:
    void ustaw(const std::string& klucz, Wartosc wartosc) {
        dane_[klucz] = std::move(wartosc);
    }

    Wartosc pobierz(const std::string& klucz) const {
        auto it = dane_.find(klucz);
        if (it == dane_.end()) throw RegistryKeyError{klucz};
        return it->second;
    }

    template<typename T>
        requires (std::same_as<T, int>    ||
                  std::same_as<T, double> ||
                  std::same_as<T, std::string> ||
                  std::same_as<T, bool>)
    T pobierz_jako(const std::string& klucz) const {
        const Wartosc& v = pobierz(klucz);
        if (!std::holds_alternative<T>(v)) {
            std::string typ_T;
            if constexpr (std::same_as<T, int>)         typ_T = "int";
            else if constexpr (std::same_as<T, double>)  typ_T = "double";
            else if constexpr (std::same_as<T, std::string>) typ_T = "string";
            else                                          typ_T = "bool";
            throw RegistryTypeError{klucz, typ_T, nazwa_typu(v)};
        }
        return std::get<T>(v);
    }

    bool zawiera(const std::string& klucz) const {
        return dane_.count(klucz) > 0;
    }

    bool jest_typu(const std::string& klucz, std::type_index typ) const {
        auto it = dane_.find(klucz);
        if (it == dane_.end()) return false;
        return std::visit([typ](const auto& v) {
            return std::type_index(typeid(v)) == typ;
        }, it->second);
    }

    void usun(const std::string& klucz) { dane_.erase(klucz); }

    // Iteracja w kolejności alfabetycznej
    void dla_kazdego(const std::function<void(const std::string&, const Wartosc&)>& f) const {
        std::map<std::string, const Wartosc*> posortowane;
        for (const auto& [k, v] : dane_) posortowane[k] = &v;
        for (const auto& [k, vp] : posortowane) f(k, *vp);
    }

    std::size_t rozmiar() const { return dane_.size(); }

    std::map<std::string, std::string> typy() const {
        std::map<std::string, std::string> wynik;
        for (const auto& [k, v] : dane_) wynik[k] = nazwa_typu(v);
        return wynik;
    }
};

// ---- Testy ----
int main() {
    std::cout << "=== TypeSafeRegistry ===\n";

    TypeSafeRegistry reg;
    reg.ustaw("host",    std::string{"localhost"});
    reg.ustaw("port",    8080);
    reg.ustaw("debug",   true);
    reg.ustaw("timeout", 30.5);
    reg.ustaw("nazwa",   std::string{"Serwer"});

    std::cout << std::boolalpha;
    std::cout << "host: "    << reg.pobierz_jako<std::string>("host")   << "\n";
    std::cout << "port: "    << reg.pobierz_jako<int>("port")           << "\n";
    std::cout << "debug: "   << reg.pobierz_jako<bool>("debug")         << "\n";
    std::cout << "timeout: " << reg.pobierz_jako<double>("timeout")     << "\n";

    std::cout << "\nzawiera(\"host\"): "  << reg.zawiera("host")     << "\n";
    std::cout << "zawiera(\"brak\"): "   << reg.zawiera("brak")     << "\n";
    std::cout << "rozmiar: " << reg.rozmiar() << "\n";

    // Zły typ:
    try {
        reg.pobierz_jako<double>("port");   // port to int, nie double
    } catch (const RegistryTypeError& e) {
        std::cout << "RegistryTypeError: " << e.what() << "\n";
    }

    // Brak klucza:
    try {
        reg.pobierz("nieistnieje");
    } catch (const RegistryKeyError& e) {
        std::cout << "RegistryKeyError: " << e.what() << "\n";
    }

    std::cout << "\n--- Mapa typów ---\n";
    for (const auto& [k, t] : reg.typy())
        std::cout << k << " → " << t << "\n";

    std::cout << "\n--- dla_kazdego (alfabetycznie) ---\n";
    reg.dla_kazdego([](const std::string& k, const Wartosc& v) {
        std::cout << k << " = ";
        std::visit(Overloaded{
            [](int x)               { std::cout << x; },
            [](double x)            { std::cout << x; },
            [](const std::string& s){ std::cout << "\"" << s << "\""; },
            [](bool b)              { std::cout << (b ? "true" : "false"); },
        }, v);
        std::cout << "\n";
    });

    // Usunięcie
    reg.usun("host");
    std::cout << "\nPo usunięciu 'host': rozmiar=" << reg.rozmiar() << "\n";
    std::cout << "zawiera(\"host\"): " << reg.zawiera("host") << "\n";

    return 0;
}
