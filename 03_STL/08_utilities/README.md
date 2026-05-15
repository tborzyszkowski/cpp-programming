# STL – Narzędzia ogólne

## Slajd 1: `std::pair` i `std::tuple`

```cpp
#include <utility>
#include <tuple>

// pair – para dwóch wartości (różnych typów)
std::pair<std::string, int> p = {"Anna", 30};
std::cout << p.first << " " << p.second << "\n";

auto p2 = std::make_pair("Bartek", 25);   // dedukcja typów

// Structured bindings (C++17) – dekonstrukcja
auto [imie, wiek] = p;
std::cout << imie << " " << wiek << "\n";

// tuple – krotka n wartości
std::tuple<std::string, int, double> t = {"Celina", 35, 165.5};
std::cout << std::get<0>(t) << " " << std::get<1>(t) << "\n";

auto [name, age, height] = t;   // structured binding

// Przydatne funkcje
auto t2 = std::make_tuple(1, "text", 3.14);
std::cout << std::tuple_size_v<decltype(t2)> << "\n";  // 3
```

---

## Slajd 2: `std::optional<T>` (C++17)

`optional` przechowuje **wartość lub jej brak** — bezpieczna alternatywa dla
zwracania wskaźnika, wartości sentinel (`-1`, `""`) lub pary `{bool, T}`.

```cpp
#include <optional>

// Funkcja która może nie zwrócić wyniku
std::optional<int> znajdz(const std::vector<int>& v, int cel) {
    auto it = std::find(v.begin(), v.end(), cel);
    if (it == v.end())
        return std::nullopt;  // brak wartości
    return *it;
}

std::vector<int> v = {1, 3, 5, 7};

auto wynik = znajdz(v, 5);
if (wynik.has_value())                    // lub: if (wynik)
    std::cout << "Znaleziono: " << *wynik << "\n";

// value_or – wartość domyślna gdy brak
auto r = znajdz(v, 99).value_or(-1);     // -1 gdy nie znaleziono
std::cout << r << "\n";

// value() rzuca std::bad_optional_access gdy nullopt
try {
    auto x = znajdz(v, 99).value();
} catch (const std::bad_optional_access& e) {
    std::cout << "Wyjątek: " << e.what() << "\n";
}
```

---

## Slajd 3: `std::variant<T...>` (C++17)

`variant` to **typebezpieczna unia** — przechowuje dokładnie jeden z typów listy.

```cpp
#include <variant>

std::variant<int, double, std::string> v;

v = 42;
std::cout << std::get<int>(v) << "\n";        // 42

v = 3.14;
std::cout << std::get<double>(v) << "\n";     // 3.14

v = std::string{"Hello"};
std::cout << std::get<std::string>(v) << "\n"; // Hello

// Sprawdzenie aktywnego typu
std::cout << v.index() << "\n";              // 2 (string jest na pozycji 2)
if (std::holds_alternative<std::string>(v))
    std::cout << "To jest string\n";

// std::visit – wywołanie funkcji dla aktywnego typu
std::visit([](const auto& val){
    std::cout << "Wartość: " << val << "\n";
}, v);

// Wzorzec visitor z overload
struct Visitor {
    void operator()(int n)               const { std::cout << "int: " << n << "\n"; }
    void operator()(double d)            const { std::cout << "double: " << d << "\n"; }
    void operator()(const std::string& s) const { std::cout << "string: " << s << "\n"; }
};
std::visit(Visitor{}, v);
```

---

## Slajd 4: `std::any` (C++17)

`any` przechowuje **dowolny typ** z kontrolą w czasie wykonania (type erasure).

```cpp
#include <any>

std::any a = 42;
std::cout << std::any_cast<int>(a) << "\n";   // 42

a = std::string{"Hello"};
std::cout << std::any_cast<std::string>(a) << "\n";

a = 3.14;

// Bezpieczne wyłuskanie przez wskaźnik
if (auto* p = std::any_cast<double>(&a))
    std::cout << "double: " << *p << "\n";

// Wyjątek przy złym typie
try {
    std::any_cast<int>(a);   // rzuca std::bad_any_cast
} catch (const std::bad_any_cast& e) {
    std::cout << "bad_any_cast: " << e.what() << "\n";
}

// Sprawdzenie typu
std::cout << a.type().name() << "\n";   // zależne od kompilatora
std::cout << "has_value: " << a.has_value() << "\n";
a.reset();  // wyczyść
```

> `any` jest znacznie cięższy niż `variant` — używaj `variant` gdy typy są znane w czasie kompilacji.

---

## Slajd 5: `std::chrono` — pomiar czasu

```cpp
#include <chrono>

using namespace std::chrono;

// Pomiar czasu wykonania
auto start = steady_clock::now();

// ... kod do zmierzenia ...
volatile long suma = 0;
for (long i = 0; i < 10'000'000; ++i) suma += i;

auto stop = steady_clock::now();
auto czas = duration_cast<milliseconds>(stop - start);
std::cout << "Czas: " << czas.count() << " ms\n";

// Czas z różnymi jednostkami
auto t = 2h + 30min + 15s;
std::cout << duration_cast<seconds>(t).count() << " sekund\n";  // 9015

// Czas systemowy
auto teraz = system_clock::now();
auto czas_t = system_clock::to_time_t(teraz);
std::cout << std::ctime(&czas_t);  // czytelna data/czas

// Literały (C++14)
auto deadline = steady_clock::now() + 500ms;
```

---

## Slajd 6: `std::filesystem` (C++17)

```cpp
#include <filesystem>
namespace fs = std::filesystem;

// Informacje o ścieżce
fs::path p = "/home/user/projekt/main.cpp";
std::cout << p.filename()   << "\n";  // main.cpp
std::cout << p.stem()       << "\n";  // main
std::cout << p.extension()  << "\n";  // .cpp
std::cout << p.parent_path()<< "\n";  // /home/user/projekt

// Tworzenie katalogów
fs::create_directories("output/data");

// Sprawdzenie istnienia
if (fs::exists("plik.txt"))
    std::cout << "Rozmiar: " << fs::file_size("plik.txt") << " B\n";

// Iteracja po katalogu
for (const auto& entry : fs::directory_iterator(".")) {
    if (entry.is_regular_file())
        std::cout << entry.path().filename() << "\n";
}

// Rekurencyjna iteracja
for (const auto& entry : fs::recursive_directory_iterator("src")) {
    if (entry.path().extension() == ".cpp")
        std::cout << entry.path() << "\n";
}

// Kopiowanie, przenoszenie, usuwanie
fs::copy("plik.txt", "kopia.txt");
fs::rename("stara.txt", "nowa.txt");
fs::remove("niepotrzebny.txt");
```

---

## Pliki źródłowe

| Plik | Opis |
|------|------|
| [`src/main.cpp`](src/main.cpp) | Demonstracja `optional`, `variant`, `any`, `chrono`, `filesystem` |
| [`utilities_diagram.puml`](utilities_diagram.puml) | Mapa narzędzi ogólnych C++17/20 |
| [`utilities_diagram.png`](utilities_diagram.png) | Wygenerowany diagram PNG |

![Narzędzia ogólne C++17/20](utilities_diagram.png)
