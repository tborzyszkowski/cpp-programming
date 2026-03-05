# C++ – Programowanie Obiektowe (OOP)

Materiały dydaktyczne do wykładu z programowania obiektowego w C++.

## Spis treści

| Nr | Temat | Katalog | Kluczowe pojęcia |
|----|-------|---------|-----------------|
| 01 | [Klasa i Obiekt](01_classes/README.md) | `01_classes/` | klasa, obiekt, enkapsulacja, `this` |
| 02 | [Testy Jednostkowe (Google Test)](02_unit_tests/README.md) | `02_unit_tests/` | GTest, `TEST_F`, fixture, asercje |
| 03 | [Kopie Płytkie i Głębokie](03_copy/README.md) | `03_copy/` | shallow copy, deep copy, Reguła Trzech |
| 04 | [Konstruktory i Destruktory](04_constructors/README.md) | `04_constructors/` | ctor domyślny, kopiujący, delegujący, dtor |
| 05 | [Move Semantics](05_move_semantics/README.md) | `05_move_semantics/` | r-value, `&&`, `std::move`, Reguła Pięciu |
| 06 | [Stałe Metody i Obiekty](06_const/README.md) | `06_const/` | `const`, `constexpr`, `const&` |
| 07 | [Składowe Statyczne](07_static_members/README.md) | `07_static_members/` | `static`, metody klasowe, Singleton |
| 08 | [Funkcje i Klasy Zaprzyjaźnione](08_friend/README.md) | `08_friend/` | `friend`, enkapsulacja, `operator<<` |
| 09 | [Stos i Sterta](09_stack_heap/README.md) | `09_stack_heap/` | `new/delete`, RAII, smart pointers, brak GC |

---

## Struktura katalogów

```
01_OOP/src/
├── 01_classes/
│   ├── README.md               ← materiał wykładowy
│   ├── class_diagram.puml      ← diagram PlantUML
│   └── src/
│       ├── Point.h
│       ├── Rectangle.h
│       └── main.cpp
├── 02_unit_tests/
│   ├── README.md
│   ├── unit_test_diagram.puml
│   ├── CMakeLists.txt          ← Google Test przez FetchContent
│   └── src/
│       ├── Calculator.h
│       └── calculator_tests.cpp
├── 03_copy/
│   ├── README.md
│   ├── copy_diagram.puml
│   └── src/
│       ├── Buffer.h            ← ShallowBuffer + DeepBuffer
│       └── main.cpp
├── 04_constructors/
│   ├── README.md
│   ├── constructor_diagram.puml
│   ├── lifecycle_diagram.puml
│   └── src/
│       ├── Car.h
│       └── main.cpp
├── 05_move_semantics/
│   ├── README.md
│   ├── move_diagram.puml
│   └── src/
│       ├── Buffer.h
│       └── main.cpp
├── 06_const/
│   ├── README.md
│   ├── const_diagram.puml
│   └── src/
│       ├── Circle.h
│       └── main.cpp
├── 07_static_members/
│   ├── README.md
│   ├── static_diagram.puml
│   └── src/
│       ├── BankAccount.h
│       └── main.cpp
├── 08_friend/
│   ├── README.md
│   ├── friend_diagram.puml
│   └── src/
│       ├── Car.h
│       └── main.cpp
└── 09_stack_heap/
    ├── README.md
    ├── memory_diagram.puml
    └── src/
        ├── MemoryDemo.h
        └── main.cpp
```

---

## Kompilacja

### Wymagania

- Kompilator C++17: **GCC 7+**, **Clang 5+** lub **MSVC 2017+**  
- (Opcjonalnie) **CMake 3.14+** – dla testów jednostkowych (gtest)
- (Opcjonalnie) **PlantUML** – do generowania diagramów PNG

### Kompilacja przykładów (g++)

```bash
# Temat 1: Klasy
g++ -std=c++17 -o 01_classes/classes 01_classes/src/main.cpp

# Temat 3: Kopie
g++ -std=c++17 -o 03_copy/copy 03_copy/src/main.cpp

# Temat 4: Konstruktory
g++ -std=c++17 -o 04_constructors/ctor 04_constructors/src/main.cpp

# Temat 5: Move semantics
g++ -std=c++17 -o 05_move_semantics/move 05_move_semantics/src/main.cpp

# Temat 6: const
g++ -std=c++17 -o 06_const/constdemo 06_const/src/main.cpp

# Temat 7: Static members
g++ -std=c++17 -o 07_static_members/static_demo 07_static_members/src/main.cpp

# Temat 8: Friend
g++ -std=c++17 -o 08_friend/friend 08_friend/src/main.cpp

# Temat 9: Stack & Heap
g++ -std=c++17 -o 09_stack_heap/memory 09_stack_heap/src/main.cpp
```

### Testy jednostkowe (temat 02)

```bash
cd 02_unit_tests
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

### Generowanie diagramów PlantUML

```bash
# Zainstaluj PlantUML (Java wymagane):
# https://plantuml.com/download

# Wygeneruj PNG z diagramu:
plantuml 01_classes/class_diagram.puml

# Wszystkie diagramy naraz (PowerShell):
Get-ChildItem -Recurse -Filter "*.puml" | ForEach-Object { plantuml $_.FullName }
```

### Generowanie prezentacji (Pandoc)

```bash
# Instalacja Pandoc + reveal.js
# Slajdy HTML:
pandoc 01_classes/README.md -t revealjs -s -o 01_classes/slides.html

# Slajdy PPTX:
pandoc 01_classes/README.md -o 01_classes/slides.pptx
```

---

## Literatura i linki

- [cppreference.com](https://en.cppreference.com) – pełna dokumentacja C++
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) – dobre praktyki
- [Google Test](https://github.com/google/googletest) – framework testów
- [PlantUML](https://plantuml.com) – generowanie diagramów
