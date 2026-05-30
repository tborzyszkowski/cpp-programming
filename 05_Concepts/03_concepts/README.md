# Concepts – C++20

## Slajd 1: Motywacja – dlaczego Concepts?

Problem: Szablony C++ bez concepts mają **niejasne komunikaty błędów**:

```cpp
// Bez concepts – stary styl (C++17):
template<typename T>
T policz(T a, T b) { return a + b; }

policz("hello", "world");   // ??? Dwa wskaźniki... kompiluje się!
// Wynik: niezdefiniowane zachowanie (dodaje wskaźniki)

// A gdy dodamy std::sort do czegoś bez operatora<:
struct Foo {};
std::sort(v.begin(), v.end());  // v = vector<Foo>
// Błąd: 50-100 linii o operator<, compare, swap...
// Nikt nie wie, co jest nie tak!
```

```cpp
// Z Concepts – styl C++20:
template<std::addable T>       // jawny wymóg
T policz(T a, T b) { return a + b; }

policz("hello", "world");
// Błąd kompilacji: "pointer does not satisfy concept 'addable'"
// Jeden wiersz! Wiadomo, co trzeba naprawić.
```

Concepts to **formalne kontrakty typów** – kompilator sprawdza je *w miejscu wywołania*,
nie wewnątrz szablonu.

---

## Slajd 2: Składnia concept – definiowanie

```cpp
#include <concepts>

// Definicja concept – wyrażenie bool w czasie kompilacji
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;   // a+b musi być konwertowalny do T
};

// Concept prostszy – tylko sprawdzenie warunku
template<typename T>
concept Calkowity = std::is_integral_v<T>;   // alias na type_trait

// Concept z wieloma wymaganiami
template<typename T>
concept Kontener = requires(T c) {
    c.begin();        // musi mieć begin()
    c.end();          // musi mieć end()
    c.size();         // musi mieć size()
    typename T::value_type;   // musi mieć zagnieżdżony typ value_type
};

// Concept z requirements na składowe
template<typename T>
concept Serializowalny = requires(T x, std::ostream& os) {
    { x.serialize() }  -> std::convertible_to<std::string>;
    { os << x }        -> std::same_as<std::ostream&>;
};
```

---

## Slajd 3: Klauzula `requires` – cztery sposoby użycia

```cpp
// Sposób 1: requires po template<>
template<typename T>
    requires std::integral<T>
T policz(T a, T b) { return a + b; }

// Sposób 2: requires po sygnaturze
template<typename T>
T policz2(T a, T b) requires std::integral<T>
{ return a + b; }

// Sposób 3: concept zamiast typename
template<std::integral T>
T policz3(T a, T b) { return a + b; }

// Sposób 4: abbreviated function template (C++20) – najkrótszy
auto policz4(std::integral auto a, std::integral auto b) {
    return a + b;
}

// Wszystkie cztery są równoważne!
// policz(3, 4);    // OK
// policz(3.5, 4.5); // BŁĄD: double nie spełnia integral
```

---

## Slajd 4: Wyrażenia `requires` – szczegóły

```cpp
// requires expression – blok sprawdzający właściwości T
template<typename T>
concept Iterator = requires(T it) {
    // Proste wymaganie – wyrażenie musi być poprawne
    *it;
    ++it;

    // Wymaganie typowe – musi istnieć zagnieżdżony typ
    typename T::value_type;

    // Wymaganie złożone – wyrażenie i typ wyniku
    { *it } -> std::same_as<typename T::value_type&>;

    // Wymaganie wyjątku (noexcept)
    { ++it } noexcept;
};

// Zagnieżdżone requires
template<typename T>
concept ForwardIterator = Iterator<T>
    && requires(T it) {
        it++;          // post-increment też musi działać
        T{it};         // musi być copy-constructible
    };

// Disjunkcja i koniunkcja concepts
template<typename T>
concept Liczba = std::integral<T> || std::floating_point<T>;

template<typename T>
concept Calkowity32bit = std::integral<T> && (sizeof(T) == 4);
```

---

## Slajd 5: Wbudowane concepts – biblioteka standardowa

```cpp
#include <concepts>

// ---- Typy podstawowe ----
std::same_as<T, U>           // T i U są dokładnie tym samym typem
std::convertible_to<T, U>    // T jest konwertowalny do U
std::derived_from<T, B>      // T dziedziczy po B

// ---- Typy liczbowe ----
std::integral<T>             // typ całkowity (int, long, char...)
std::signed_integral<T>      // typ całkowity ze znakiem
std::unsigned_integral<T>    // typ całkowity bez znaku
std::floating_point<T>       // float, double, long double

// ---- Operacje na typach ----
std::movable<T>              // można przenosić (move)
std::copyable<T>             // można kopiować i przenosić
std::regular<T>              // copyable + equality_comparable
std::semiregular<T>          // copyable + defaultconstructible

// ---- Porównania ----
std::equality_comparable<T>            // operator==
std::totally_ordered<T>               // <, >, <=, >=
std::three_way_comparable<T>          // operator<=>

// ---- Callable ----
std::invocable<F, Args...>   // F można wywołać z Args
std::regular_invocable<F, Args...>  // jw + nie zmienia stanu

// ---- Iteratory / zakresy ----
std::input_iterator<T>
std::forward_iterator<T>
std::random_access_iterator<T>
std::ranges::range<T>
std::ranges::sized_range<T>
```

---

## Slajd 6: Concepts w `std::ranges`

```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <list>

// std::ranges::sort przyjmuje RandomAccessRange
// std::sort przyjmuje RandomAccessIterator

void demo() {
    std::vector<int> v{3, 1, 4, 1, 5, 9};
    std::ranges::sort(v);    // OK: vector to random_access_range
    // std::ranges::sort(std::list<int>{});  // BŁĄD: list to bidirectional, nie random!

    // Ranges pipeline z views (lazy)
    auto parzyste_kwadraty = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });

    for (int x : parzyste_kwadraty)
        std::cout << x << " ";
    std::cout << "\n";
}

// Własna funkcja wymagająca range
template<std::ranges::input_range R>
void drukuj_zakres(const R& r) {
    for (const auto& x : r)
        std::cout << x << " ";
    std::cout << "\n";
}
```

---

## Slajd 7: Subsumpcja – hierarchia concepts

```cpp
// Concepts tworzą hierarchię – kompilator wybiera najbardziej szczegółowy
template<typename T>
concept Calkowity = std::integral<T>;

template<typename T>
concept CalkowityBezZnaku = std::unsigned_integral<T>;

// unsigned_integral subsumuje integral (jest bardziej szczegółowy)

template<std::integral T>
void f(T x) { std::cout << "integral: " << x << "\n"; }

template<std::unsigned_integral T>
void f(T x) { std::cout << "unsigned integral: " << x << "\n"; }

// Subsumpcja: unsigned_integral ⊆ integral
// Kompilator wybiera bardziej szczegółowe przeciążenie
// f(42u);   → "unsigned integral: 42"  (nie "integral: 42")
// f(-5);    → "integral: -5"           (signed nie spełnia unsigned)

// Własna hierarchia
template<typename T>
concept Ksztalt = requires(T k) { { k.pole() } -> std::floating_point; };

template<typename T>
concept KsztaltZ_Obwodem = Ksztalt<T>     // Ksztalt jest bazowym concept
    && requires(T k) { { k.obwod() } -> std::floating_point; };
```

---

## Slajd 8: Porównanie – SFINAE vs Concepts

| Aspekt | SFINAE (C++11/17) | Concepts (C++20) |
|--------|-------------------|-----------------|
| Czytelność definicji | Niska | Wysoka |
| Komunikat błędu | Wieloliniowy | Jasny, 1-2 linii |
| Subsumpcja (hierarchia) | Brak | Automatyczna |
| Wielokrotne użycie | Trudne | `concept Nazwa = ...` |
| Diagnostyka w IDE | Słaba | Dobra |
| Kompatybilność | C++11+ | C++20+ |

```cpp
// SFINAE (stary styl) – trudny w czytaniu:
template<typename T,
    std::enable_if_t<std::is_integral_v<T> &&
                     std::is_signed_v<T>, int> = 0>
void f(T x) { std::cout << x; }

// Concepts (nowy styl) – czytelny:
template<std::signed_integral T>
void f(T x) { std::cout << x; }

// Lub najkrótszy zapis:
void f(std::signed_integral auto x) { std::cout << x; }
```
