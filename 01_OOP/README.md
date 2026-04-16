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
01_OOP/
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

- Kompilator C++17: **GCC 7+** / **MinGW-w64** (Windows), **Clang 5+** lub **MSVC 2017+**
- **CMake 3.14+** – dla testów jednostkowych (Google Test przez FetchContent)
- **Java 8+** – do uruchamiania `plantuml.jar`
- (Opcjonalnie) rozszerzenie VS Code **PlantUML** (`jebbs.plantuml`) – skrypt wykrywa jego `plantuml.jar` automatycznie

### Skrypt PowerShell `build.ps1` (zalecane)

Skrypt [`build.ps1`](../build.ps1) w katalogu głównym projektu automatyzuje wszystkie zadania:

```powershell
# Z katalogu głównego projektu:

# Wszystko naraz
.\build.ps1

# Tylko diagramy PlantUML → PNG
.\build.ps1 -Task diagrams

# Tylko kompilacja + uruchomienie programów C++
.\build.ps1 -Task programs

# Tylko testy jednostkowe (CMake + Google Test)
.\build.ps1 -Task tests
```

Skrypt automatycznie wyszukuje `plantuml.jar` (rozszerzenie VS Code, Chocolatey, katalog projektu) oraz sprawdza dostępność `g++` i `cmake`.

### Kompilacja ręczna (g++)

```bash
# Temat 1: Klasy
g++ -std=c++17 -Wall -Wextra -o 01_classes/src/classes.exe 01_classes/src/main.cpp

# Temat 3: Kopie
g++ -std=c++17 -Wall -Wextra -o 03_copy/src/copy.exe 03_copy/src/main.cpp

# Temat 4: Konstruktory
g++ -std=c++17 -Wall -Wextra -o 04_constructors/src/constructors.exe 04_constructors/src/main.cpp

# Temat 5: Move semantics
g++ -std=c++17 -Wall -Wextra -o 05_move_semantics/src/move_semantics.exe 05_move_semantics/src/main.cpp

# Temat 6: const
g++ -std=c++17 -Wall -Wextra -o 06_const/src/const_demo.exe 06_const/src/main.cpp

# Temat 7: Static members
g++ -std=c++17 -Wall -Wextra -o 07_static_members/src/static_members.exe 07_static_members/src/main.cpp

# Temat 8: Friend
g++ -std=c++17 -Wall -Wextra -o 08_friend/src/friend_demo.exe 08_friend/src/main.cpp

# Temat 9: Stack & Heap
g++ -std=c++17 -Wall -Wextra -o 09_stack_heap/src/stack_heap.exe 09_stack_heap/src/main.cpp
```

### Testy jednostkowe (temat 02)

Zalecane – przez skrypt:

```powershell
.\build.ps1 -Task tests
```

Ręcznie (MinGW):

```bash
cd 02_unit_tests
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
cd build && ctest --output-on-failure
```

### Generowanie diagramów PlantUML

Pliki PNG są generowane automatycznie przez `build.ps1 -Task diagrams`.

Wygenerowane diagramy:

| Plik `.puml` | Wygenerowany `.png` |
|---|---|
| `01_classes/class_diagram.puml` | `01_classes/class_diagram.png` |
| `02_unit_tests/unit_test_diagram.puml` | `02_unit_tests/unit_test_diagram.png` |
| `03_copy/copy_diagram.puml` | `03_copy/copy_diagram.png` |
| `04_constructors/constructor_diagram.puml` | `04_constructors/constructor_diagram.png` |
| `04_constructors/lifecycle_diagram.puml` | `04_constructors/lifecycle_diagram.png` |
| `05_move_semantics/move_diagram.puml` | `05_move_semantics/move_diagram.png` |
| `06_const/const_diagram.puml` | `06_const/const_diagram.png` |
| `07_static_members/static_diagram.puml` | `07_static_members/static_diagram.png` |
| `08_friend/friend_diagram.puml` | `08_friend/friend_diagram.png` |
| `09_stack_heap/memory_diagram.puml` | `09_stack_heap/memory_diagram.png` |

Kompilacja ręczna (wymaga Java):

```bash
# Pojedynczy plik:
java -jar plantuml.jar -tpng 01_classes/class_diagram.puml

# Wszystkie diagramy (PowerShell):
Get-ChildItem -Recurse -Filter "*.puml" | ForEach-Object {
    java -jar plantuml.jar -tpng -o $_.DirectoryName $_.FullName
}
```

---

## Literatura

1. B. Stroustrup, *The C++ Programming Language*, 4th ed., Addison-Wesley, 2013 – rozdz. 16–17 (klasy, konstruktory, kopiowanie).
2. S. Meyers, *Effective C++*, 3rd ed., Addison-Wesley, 2005 – pozycje 5–12 (zarządzanie zasobami, kopiowanie).
3. S. Meyers, *Effective Modern C++*, O'Reilly, 2014 – pozycje 17, 23, 29 (move semantics, smart pointery).
4. N. Josuttis, *The C++ Standard Library*, 2nd ed., Addison-Wesley, 2012 – rozdz. 2 (wprowadzenie do typów i obiektów).
5. ISO/IEC 14882:2020 (C++20) – §11 „Classes", §15 „Special member functions".

## Linki

- [cppreference.com](https://en.cppreference.com) – pełna dokumentacja C++
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) – dobre praktyki (C.ctor, C.copy, R.smart)
- [Google Test](https://github.com/google/googletest) – framework testów jednostkowych
- [PlantUML](https://plantuml.com) – generowanie diagramów UML
- [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) – pobieranie zależności
