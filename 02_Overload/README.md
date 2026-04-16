# Przeciążanie Operatorów w C++

## Spis treści

| Rozdział | Temat | Pliki źródłowe |
|----------|-------|----------------|
| [01 – Wprowadzenie](01_introduction/README.md) | Idea, motywacja, czym jest przeciążanie | [`src/main.cpp`](01_introduction/src/main.cpp) |
| [02 – Przegląd operatorów](02_operators_overview/README.md) | Które operatory istnieją, które przeciążamy | [`src/main.cpp`](02_operators_overview/src/main.cpp) |
| [03 – Składnia i zasady](03_syntax_and_rules/README.md) | Jak pisać przeciążenia, metoda vs funkcja wolna | [`src/Complex.h`](03_syntax_and_rules/src/Complex.h) |
| [04 – Przypadki szczególne](04_special_cases/README.md) | Jednoargumentowe, ++/--, porównania, I/O | [`src/main.cpp`](04_special_cases/src/main.cpp) |
| [05 – Przykłady](05_examples/README.md) | Klasa `Fraction` – pełny zestaw operatorów | [`src/Fraction.h`](05_examples/src/Fraction.h) |
| [06 – Vec3d](06_vec3d/README.md) | Większy przykład: wektor 3D | [`src/Vec3d.h`](06_vec3d/src/Vec3d.h) |

---

## Cel modułu

Po ukończeniu tego modułu student będzie potrafił:

- wyjaśnić, **czym jest przeciążanie operatorów** i dlaczego C++ na to pozwala,
- wskazać, **które operatory można**, a których **nie wolno** przeciążać i uzasadnić dlaczego,
- napisać przeciążenie operatora jako **metodę klasy** lub **wolną funkcję** (z `friend`),
- obsłużyć **szczególne przypadki**: operatory jednoargumentowe, prefix/postfix `++`/`--`, operatory wejścia/wyjścia,
- zaprojektować klasę z **kompletnym, spójnym zestawem** przeciążonych operatorów.

---

## Szybki przegląd

```
Wyrażenie:  a + b
                │
                ▼ kompilator tłumaczy na jedno z:
         a.operator+(b)         ← forma metody składowej
         operator+(a, b)        ← forma wolnej funkcji
```

Przeciążanie operatorów to nic innego jak **definiowanie funkcji** o specjalnej nazwie `operatorX`.  
Nie zmieniamy zasad języka — korzystamy z istniejącego mechanizmu wywoływania funkcji.

---

## Literatura

1. B. Stroustrup, *The C++ Programming Language*, 4th ed., Addison-Wesley, 2013 – rozdz. 18 „Operator Overloading".
2. S. Meyers, *Effective C++*, 3rd ed., Addison-Wesley, 2005 – pozycje 23–27.
3. N. Josuttis, *The C++ Standard Library*, 2nd ed., Addison-Wesley, 2012 – rozdz. 3.
4. cppreference.com – [Operator overloading](https://en.cppreference.com/w/cpp/language/operators)
5. ISO/IEC 14882:2020 (C++20) – §12 „Overloading".
