# cpp-programming

Materiały dydaktyczne do zajęć z programowania w C++.

## Szybki start

> Wymagania: **g++ (C++17)**, **CMake 3.14+**, **Java** (do PlantUML)

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

| Moduł | Temat |
|-------|-------|
| [01_OOP](01_OOP/README.md) | Programowanie obiektowe w C++ |
