# Szablony, Concepts i Type Erasure w C++

## Spis treści

| Rozdział | Temat | Pliki źródłowe |
|----------|-------|----------------|
| [01 – Historia i motywacja](01_history/README.md) | Makra, `void*`, Ada/ML, programowanie generyczne, droga do C++98 i dalej | [`src/main.cpp`](01_history/src/main.cpp) |
| [02 – Szablony – składnia i mechanizmy](02_templates/README.md) | Szablony funkcji i klas, specjalizacje, SFINAE, variadic templates, type traits | [`src/main.cpp`](02_templates/src/main.cpp) |
| [03 – Concepts (C++20)](03_concepts/README.md) | `requires`, ograniczenia, predefined concepts, `std::ranges`, refining interfaces | [`src/main.cpp`](03_concepts/src/main.cpp) |
| [04 – Duck Typing w C++](04_duck_typing/README.md) | Strukturalne typowanie, SFINAE jako duck typing, concepts jako jawny kontrakt | [`src/main.cpp`](04_duck_typing/src/main.cpp) |
| [05 – Type Erasure](05_type_erasure/README.md) | Wirtualne funkcje, `std::function`, `std::any`, `std::variant`, własna implementacja | [`src/main.cpp`](05_type_erasure/src/main.cpp) |
| [06 – Moduły C++20](06_modules/README.md) | `module`, `import`, `export`, partycje, porównanie z nagłówkami, modularyzacja bibliotek | [`src/`](06_modules/src/) |
| [Zadania](exercises/README.md) | 3 zadania z rozwiązaniami | [`solutions/`](exercises/solutions/) |

---

## Cel modułu

Po ukończeniu tego modułu student będzie potrafił:

- wyjaśnić **ewolucję programowania generycznego** w C++ — od makr przez szablony do concepts,
- pisać szablony funkcji i klas z **częściową i pełną specjalizacją**,
- korzystać z mechanizmu **SFINAE** i `std::enable_if` (styl pre-C++20),
- definiować i stosować **concepts** jako precyzyjne kontrakty typów (C++20),
- rozumieć **duck typing** — kiedy jest niejawny (szablony), a kiedy jawny (concepts),
- implementować **type erasure** przy użyciu wirtualnych funkcji, `std::any`, `std::variant` i własnych technik,
- tworzyć i importować **moduły C++20** zamiast plików nagłówkowych.

---

## Mapa tematyczna

```
                     ┌─────────────────────────────────────┐
                     │      Programowanie generyczne        │
                     │  „pisz algorytm raz dla każdego T"  │
                     └──────────────┬──────────────────────┘
                                    │
              ┌─────────────────────┼────────────────────────┐
              ▼                     ▼                         ▼
   ┌──────────────────┐  ┌──────────────────┐   ┌─────────────────────┐
   │    Szablony      │  │    Concepts       │   │    Type Erasure     │
   │  (C++98/11/14)   │  │    (C++20)        │   │  (runtime polimorf.)│
   │                  │  │                   │   │                     │
   │ template<T>      │  │ concept Sortable  │   │ virtual dispatch    │
   │ SFINAE           │  │ requires(T x)     │   │ std::function       │
   │ variadic         │  │   { x < x; }      │   │ std::any            │
   │ type traits      │  │ std::ranges       │   │ std::variant        │
   └──────────────────┘  └──────────────────┘   └─────────────────────┘
              │                     │
              └─────────┬───────────┘
                        ▼
           ┌────────────────────────┐
           │      Duck Typing       │
           │  niejawny (szablony)   │
           │  jawny (concepts)      │
           └────────────────────────┘
                        │
                        ▼
           ┌────────────────────────┐
           │     Moduły C++20       │
           │  lepsza enkapsulacja   │
           │  szybsza kompilacja    │
           └────────────────────────┘
```

---

## Literatura

1. B. Stroustrup, *The C++ Programming Language*, 4th ed., §23–26 „Templates".
2. D. Vandevoorde, N. Josuttis, D. Gregor, *C++ Templates: The Complete Guide*, 2nd ed., 2017.
3. B. Stroustrup, *A Tour of C++*, 3rd ed., §7 „Concepts and Generic Programming", §16 „Modules".
4. A. Stepanov & P. McJones, *Elements of Programming*, Addison-Wesley, 2009.
5. cppreference.com – [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints), [Modules](https://en.cppreference.com/w/cpp/language/modules)
6. ISO/IEC 14882:2020 (C++20) – §13 „Templates", §10 „Modules".
7. S. Meyers, *Effective Modern C++*, Items 1–4 (dedukcja typów), 27–28 (forwarding).
