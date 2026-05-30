# Wyrażenia Lambda w C++

## Spis treści

| Rozdział | Temat | Pliki źródłowe |
|----------|-------|----------------|
| [01 – Historia i motywacja](01_history/README.md) | Wskaźniki na funkcje, funktory, geneza lambd w C++11 | [`src/main.cpp`](01_history/src/main.cpp) |
| [02 – Składnia i komponenty](02_syntax/README.md) | Anatomy lambdy, dedukcja typów, `decltype`, typ domknięcia | [`src/main.cpp`](02_syntax/src/main.cpp) |
| [03 – Przechwycenie (domknięcia)](03_captures/README.md) | `[=]`, `[&]`, mieszane, `mutable`, czas życia, pułapki | [`src/main.cpp`](03_captures/src/main.cpp) |
| [04 – Zaawansowane cechy](04_advanced/README.md) | Lambdy generyczne (C++14), `constexpr` (C++17), szablonowe (C++20), rekurencja | [`src/main.cpp`](04_advanced/src/main.cpp) |
| [05 – Zastosowania praktyczne](05_practical/README.md) | STL, callbacki, programowanie funkcyjne, GUI, DSL, korutyny | [`src/main.cpp`](05_practical/src/main.cpp) |
| [Zadania](exercises/README.md) | 3 zadania z rozwiązaniami | [`solutions/`](exercises/solutions/) |

---

## Cel modułu

Po ukończeniu tego modułu student będzie potrafił:

- wyjaśnić **ewolucję callables** w C++ — od wskaźników na funkcje przez funktory do lambd,
- czytać i pisać lambdy ze wszystkimi rodzajami **przechwycenia zmiennych**,
- stosować lambdy **generyczne i szablonowe** (C++14/C++20),
- rozpoznawać i unikać typowych **pułapek** (dangling references, domknięcia w pętlach),
- używać lambd efektywnie jako **predykatów, callbacków, strategii i komparatorów**,
- rozumieć relację lambda ↔ funktor ↔ `std::function` ↔ `auto`.

---

## Dlaczego lambdy zmieniły C++

Przed C++11 każdy callable wymagający stanu musiał być osobną klasą:

```cpp
// C++03 – osobna klasa tylko po to, by przefiltrować wektor
struct WiekszyNiz {
    int prog;
    explicit WiekszyNiz(int p) : prog(p) {}
    bool operator()(int x) const { return x > prog; }
};

std::vector<int> v = {1, 3, 5, 7, 9};
std::remove_if(v.begin(), v.end(), WiekszyNiz{5});
```

```cpp
// C++11 – to samo w jednej linii
std::remove_if(v.begin(), v.end(), [](int x){ return x > 5; });
```

Lambda eliminuje odległość między **miejscem użycia** a **definicją logiki** —
kod jest czytelniejszy i mniej podatny na błędy.

---

## Ewolucja lambd — jeden rzut oka

```
C++11  ──▶  podstawowa składnia, przechwycenie, mutable
C++14  ──▶  lambdy generyczne (auto parametry), inicjalizatory przechwycenia
C++17  ──▶  constexpr lambda, *this przez wartość
C++20  ──▶  lambdy szablonowe, lambdy w unevaluated contexts, default capture w [=] bez this
C++23  ──▶  dedukcja this (explicit object parameter), lambdy w static context
```

---

## Szybki przewodnik składni

```
[ przechwycenie ] ( parametry ) specyfikatory -> typ { ciało }
     │                │              │            │      │
     │                │              │            │      └── logika
     │                │              │            └───────── typ zwracany (opcjonalny)
     │                │              └────────────────────── mutable / constexpr / noexcept
     │                └───────────────────────────────────── lista parametrów (opcjonalna)
     └────────────────────────────────────────────────────── co przechwycić z otoczenia
```

---

## Literatura

1. B. Stroustrup, *The C++ Programming Language*, 4th ed., §11.4 „Lambda Expressions".
2. S. Meyers, *Effective Modern C++*, Items 31–34 (rozdział o lambdach).
3. B. Filipek, *C++17 in Detail*, rozdz. 6 „Lambda Expressions".
4. cppreference.com – [Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)
5. ISO/IEC 14882:2020 (C++20) – §7.5.5 „Lambda expressions".
6. P. Sommerlad, *Lambdas in C++11* – ACCU 2013 (prezentacja).
