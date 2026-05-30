# Moduły C++20

## Slajd 1: Problemy z `#include`

Przez 50 lat C++ korzystał z systemu nagłówków odziedziczonego z C. Niesie to ze sobą poważne problemy:

```cpp
// Plik: main.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include "MojaKlasa.h"
// ↑ Preprocesor KOPIUJE zawartość każdego pliku.
// <iostream> to ~10 000 linii kodu wklejanych dosłownie do każdego .cpp!

// Problem 1: POWOLNA KOMPILACJA
// Każda jednostka translacji kompiluje te same nagłówki od nowa.
// W dużych projektach: 80-90% czasu kompilacji to nagłówki!

// Problem 2: WYCIEKI MAKR
#define MAX 100   // w MojaKlasa.h
// MAX zanieczyszcza globalną przestrzeń nazw!

// Problem 3: KOLEJNOŚĆ INCLUDOWANIA
// Błędy trudne do znalezienia, gdy nagłówki są w złej kolejności

// Problem 4: BRAK ENKAPSULACJI
// Wszystkie symbole z nagłówka są widoczne wszędzie
// Nie można "ukryć" implementacji bez pliku .cpp

// Problem 5: include guards / #pragma once – konieczne obejście
#ifndef MOJA_KLASA_H
#define MOJA_KLASA_H
// ... treść nagłówka
#endif
```

---

## Slajd 2: Moduły – nowy model kompilacji (C++20)

```cpp
// Plik: math.cppm  (lub .ixx – zależy od kompilatora)
export module math;   // deklaracja jednostki modułu

import <iostream>;    // import zależności modułu

// Eksportowane symbole – widoczne dla importera
export double kwadrat(double x) { return x * x; }
export double szescian(double x) { return x * x * x; }

// NIEEKSPORTOWANE – wewnętrzna implementacja, niewidoczna z zewnątrz!
double pomocnicza_impl(double x) { return x * 2; }   // prywatne

// Eksport klasy:
export class Kalkulator {
public:
    double dodaj(double a, double b) { return a + b; }
    double odejmij(double a, double b) { return a - b; }
};

// ─────────────────────────────────────────────
// Plik: main.cpp
import math;           // zamiast #include "math.h"
import <iostream>;     // moduł biblioteki standardowej

int main() {
    std::cout << kwadrat(5.0) << "\n";      // 25
    std::cout << szescian(3.0) << "\n";     // 27

    Kalkulator k;
    std::cout << k.dodaj(3, 4) << "\n";     // 7
    // pomocnicza_impl(5);  // BŁĄD – nie jest eksportowana
}
```

---

## Slajd 3: Partycje modułu

Duże moduły można podzielić na **partycje**:

```cpp
// Plik: geometry-shapes.cppm  (partycja "shapes" modułu "geometry")
export module geometry:shapes;

export struct Kolo  { double r; };
export struct Prostokat { double a, b; };

// ─────────────────────────────────────────────
// Plik: geometry-algorithms.cppm  (partycja "algorithms")
export module geometry:algorithms;
import :shapes;   // import innej partycji – tylko wewnątrz modułu!

export double pole(const Kolo& k)      { return 3.14 * k.r * k.r; }
export double pole(const Prostokat& p) { return p.a * p.b; }

// ─────────────────────────────────────────────
// Plik: geometry.cppm  (główna jednostka – re-eksportuje partycje)
export module geometry;
export import :shapes;        // re-eksport partycji
export import :algorithms;    // re-eksport partycji

// ─────────────────────────────────────────────
// Plik: main.cpp
import geometry;   // jeden import daje dostęp do wszystkiego

int main() {
    Kolo k{5.0};
    std::cout << pole(k) << "\n";   // z partycji algorithms
}
```

---

## Slajd 4: `module :private` – podział interfejsu i implementacji

```cpp
// Plik: mylib.cppm
export module mylib;

// ---- Interfejs publiczny (przed module :private) ----
export class Baza {
public:
    void metoda();                // deklaracja
    virtual ~Baza() = default;
};

export void funkcja_pub();        // deklaracja

// ---- Implementacja prywatna (po module :private) ----
// Uwaga: wymaga wsparcia kompilatora (GCC 14+, Clang 17+)
module :private;

void Baza::metoda() {
    // implementacja – niewidoczna dla importerów
    std::cout << "Baza::metoda()\n";
}

void funkcja_pub() {
    std::cout << "funkcja_pub()\n";
}

// Korzyść: zmiana implementacji nie wymaga rekompilacji importerów!
// (To jest niemożliwe z nagłówkami)
```

---

## Slajd 5: Porównanie – nagłówki vs moduły

| Aspekt | Nagłówki (`#include`) | Moduły (`import`) |
|--------|----------------------|-------------------|
| Szybkość kompilacji | Wolna (każdy .cpp) | **Szybka** (raz skompilowany) |
| Enkapsulacja | Brak | **Pełna** (nieeks. symbole ukryte) |
| Makra | Wyciekają | **Nie wyciekają** |
| Kolejność include | Ważna | Nie ma znaczenia |
| Narzędzia legacy | Wszechobecne | Tylko C++20+ |
| Wsparcie kompilatorów | Wszędzie | GCC 11+, Clang 16+, MSVC 2019+ |
| include guards | Konieczne | Zbędne |
| Typowe rozszerzenie | `.h`, `.hpp` | `.cppm`, `.ixx` |

**Wyniki benchmarków (Mozilla Firefox, LLVM):**
- Czas kompilacji z modułami: **40-70% krótszy** niż z nagłówkami wstępnie kompilowanymi (PCH)
- Czas przyrostowej kompilacji: **jeszcze lepszy** (zmiana implementacji ≠ recompile importerów)

---

## Slajd 6: Importowanie biblioteki standardowej

```cpp
// C++23: globalne import modułów biblioteki standardowej
import std;           // CAŁY standard w jednym imporcie (C++23)

// C++20: importowanie poszczególnych nagłówków jako moduły
import <iostream>;    // odpowiednik #include <iostream>
import <vector>;
import <algorithm>;

// Mieszanie (tymczasowe – dla kodu legacy):
#include <cstdio>     // stare nagłówki C nadal działają
import <iostream>;    // nowe moduły C++

// ─────────────────────────────────────────────
// Wzorzec: moduł biblioteki z header-compat
export module mojlib;

// Zewnętrzna zależność przez #include (gdy nie ma modułu)
#include "zewnetrzna_biblioteka.h"  // OK wewnątrz modułu

export void moja_funkcja();   // interfejs modułu
```

---

## Slajd 7: Build system i kompilacja modułów

Moduły wymagają zmiany w procesie budowania:

```bash
# GCC 14+ (eksperymentalnie):
g++ -std=c++20 -fmodules-ts -c math.cppm -o math.gcm  # precompile moduł
g++ -std=c++20 -fmodules-ts main.cpp math.gcm -o prog  # kompiluj main

# MSVC 2019+:
cl /std:c++20 /experimental:module /c math.ixx          # precompile
cl /std:c++20 /experimental:module main.cpp math.ifc    # link

# CMake 3.28+ (oficjalne wsparcie modułów):
add_library(math)
target_sources(math
  PUBLIC
    FILE_SET CXX_MODULES FILES math.cppm
)
target_compile_features(math PUBLIC cxx_std_20)
```

**Uwaga:** Ze względu na różnice wsparcia kompilatorów, kod w `src/` używa
symulacji modułów przez namespace i klasy – by działał na GCC 11+.
Komentarze pokazują, jak wyglądałby kod z pełnymi modułami.
