# Biblioteka Standardowa C++ (STL)

## Spis treści

| Rozdział | Temat | Pliki źródłowe |
|----------|-------|----------------|
| [01 – Historia i architektura](01_history/README.md) | Geneza STL, trzy filary, ewolucja od C++98 do C++20 | [`src/main.cpp`](01_history/src/main.cpp) |
| [02 – Iteratory](02_iterators/README.md) | Kategorie, operacje, zakresy, C++20 Ranges | [`src/main.cpp`](02_iterators/src/main.cpp) |
| [03 – Kontenery sekwencyjne](03_sequence_containers/README.md) | `vector`, `deque`, `list`, `array` | [`src/main.cpp`](03_sequence_containers/src/main.cpp) |
| [04 – Kontenery asocjacyjne](04_associative_containers/README.md) | `map`, `set`, `unordered_map`, własny hash | [`src/main.cpp`](04_associative_containers/src/main.cpp) |
| [05 – Algorytmy](05_algorithms/README.md) | Grupy algorytmów, sortowanie, redukcja, C++17/20 | [`src/main.cpp`](05_algorithms/src/main.cpp) |
| [06 – Funktory i lambdy](06_functors_lambdas/README.md) | `operator()`, domknięcia, `std::function`, `std::bind` | [`src/main.cpp`](06_functors_lambdas/src/main.cpp) |
| [07 – `std::string`](07_string/README.md) | SSO, `string_view`, `stringstream`, C++20 | [`src/main.cpp`](07_string/src/main.cpp) |
| [08 – Narzędzia ogólne](08_utilities/README.md) | `optional`, `variant`, `any`, `chrono`, `filesystem` | [`src/main.cpp`](08_utilities/src/main.cpp) |
| [Zadania](exercises/README.md) | 3 zadania z rozwiązaniami | [`solutions/`](exercises/solutions/) |

---

## Cel modułu

Po ukończeniu tego modułu student będzie potrafił:

- wyjaśnić **architekturę STL** — jak kontenery, algorytmy i iteratory współpracują,
- wybrać właściwy kontener do danego problemu i uzasadnić wybór złożonościowo,
- korzystać ze **standardowych algorytmów** zamiast własnych pętli,
- pisać **lambdy i funktory** jako predykaty do algorytmów,
- używać nowoczesnych narzędzi: `std::optional`, `std::variant`, `std::string_view`, `std::filesystem`.

---

## Szybki przegląd architektury

```
┌─────────────────────────────────────────────────────┐
│                   Algorytmy STL                      │
│   sort  find  transform  copy  accumulate  ...       │
└──────────────────────┬──────────────────────────────┘
                       │ operują przez
                       ▼
┌─────────────────────────────────────────────────────┐
│                   Iteratory                          │
│   begin()  end()  ++  *  ==  !=                      │
└──────┬──────────────────────────────┬───────────────┘
       │ dostarczają                  │ dostarczają
       ▼                              ▼
┌──────────────┐               ┌──────────────┐
│  Kontenery   │               │  Inne zakresy│
│  sekwencyjne │               │  (tablice,   │
│  asocjacyjne │               │   strumienie)│
└──────────────┘               └──────────────┘
```

Algorytmy **nie wiedzą**, na jakim kontenerze działają — komunikują się tylko przez iteratory.
Dzięki temu ten sam `std::sort` działa na `vector`, `deque`, tablicy C i własnym typie.

---

## Literatura

1. N. Josuttis, *The C++ Standard Library*, 2nd ed., Addison-Wesley, 2012.
2. B. Stroustrup, *A Tour of C++*, 3rd ed., Addison-Wesley, 2022 – rozdz. 11–13.
3. S. Meyers, *Effective STL*, Addison-Wesley, 2001.
4. cppreference.com – [Containers](https://en.cppreference.com/w/cpp/container), [Algorithms](https://en.cppreference.com/w/cpp/algorithm)
5. ISO/IEC 14882:2020 (C++20) – §21–29 „Library".
