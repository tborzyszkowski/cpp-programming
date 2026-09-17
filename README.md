<p align="left">
  <a href="https://github.com">
    <kbd style="background-color: #0056b3; color: white; padding: 5px 10px; border-radius: 4px; font-weight: bold; border: none; font-family: sans-serif; font-size: 13px;">🤖 AI-Assisted</kbd>
    <kbd style="background-color: #6c757d; color: white; padding: 5px 10px; border-radius: 4px; font-weight: bold; border: none; font-family: sans-serif; font-size: 13px;">Edukacja</kbd>
  </a>
</p>


# cpp-programming

Materiały dydaktyczne do zajęć z programowania w C++.
Każdy moduł zawiera slajdy w formacie Markdown, przykłady kodu źródłowego oraz diagramy PlantUML.

## Wymagania

| Narzędzie | Wersja | Zastosowanie |
|-----------|--------|--------------|
| g++ / gcc | ≥ C++17 | kompilacja przykładów |
| CMake | ≥ 3.14 | testy jednostkowe (Google Test) |
| Java | ≥ 8 | generowanie diagramów PlantUML → PNG |
| PlantUML | dowolna | plik `.jar` pobierany automatycznie przez `build.ps1` |

## Szybki start

```powershell
# Wszystko naraz (diagramy + programy + testy):
.\build.ps1

# Tylko diagramy PlantUML → PNG:
.\build.ps1 -Task diagrams

# Tylko programy C++:
.\build.ps1 -Task programs

# Tylko testy jednostkowe:
.\build.ps1 -Task tests
```

## Zawartość

| Moduł | Temat | Podtematy |
|-------|-------|-----------|
| [01_OOP](01_OOP/README.md) | Programowanie obiektowe w C++ | klasy, testy jednostkowe, kopiowanie, konstruktory, move semantics, `const`, składowe statyczne, `friend`, stos i sterta |
| [02_Overload](02_Overload/README.md) | Przeciążanie operatorów w C++ | wprowadzenie, przegląd operatorów, składnia i reguły, przypadki szczególne, przykłady, projekt `Vec3D` |
| [03_STL](03_STL/README.md) | Biblioteka Standardowa C++ (STL) | historia, iteratory, kontenery sekwencyjne, kontenery asocjacyjne, algorytmy, funktory i lambdy, `string`, narzędzia ogólne |
| [04_Lambda](04_Lambda/README.md) | Wyrażenia Lambda w C++ | historia callable, składnia, przechwycenie, zaawansowane techniki, zastosowania praktyczne |
| [05_Concepts](05_Concepts/README.md) | Szablony, Concepts i Type Erasure w C++ | historia, szablony, concepts (C++20), duck typing, type erasure, moduły (C++20) |

## Struktura modułu

Każdy moduł jest podzielony na ponumerowane podkatalogi, każdy z plikami:

```
XX_NazwaModułu/
├── README.md               ← slajdy z wyjaśnieniami
├── src/
│   └── main.cpp            ← przykład kodu
├── *_diagram.puml          ← diagram PlantUML (źródło)
└── *_diagram.png           ← wygenerowany diagram
```

Moduł `01_OOP/02_unit_tests` zawiera dodatkowo pełne środowisko CMake + Google Test.
