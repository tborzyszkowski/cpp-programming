# Operatory w C++ – przegląd

## Slajd 1: Wszystkie operatory w C++

C++ definiuje następujące kategorie operatorów:

| Kategoria | Operatory |
|-----------|-----------|
| **Arytmetyczne** | `+` `-` `*` `/` `%` |
| **Przypisanie** | `=` `+=` `-=` `*=` `/=` `%=` `&=` `\|=` `^=` `<<=` `>>=` |
| **Jednoargumentowe** | `+x` `-x` `!x` `~x` `*x` `&x` `++x` `x++` `--x` `x--` |
| **Porównania** | `==` `!=` `<` `>` `<=` `>=` |
| **Logiczne** | `&&` `\|\|` `!` |
| **Bitowe** | `&` `\|` `^` `~` `<<` `>>` |
| **Dostęp** | `[]` `()` `->` `->*` `.` `.*` |
| **Alokacja** | `new` `new[]` `delete` `delete[]` |
| **Inne** | `?:` `,` `sizeof` `typeid` `static_cast` itd. |

---

## Slajd 2: Których operatorów NIE można przeciążyć?

Sześć operatorów jest **zarezerwowanych** – nie możemy ich przeciążyć:

| Operator | Nazwa | Powód zakazu |
|----------|-------|-------------|
| `::` | Rozwiązanie zakresu | Musi działać statycznie (w czasie kompilacji) |
| `.` | Dostęp do składowej | Zmieniałoby fundamentalne reguły dostępu do obiektów |
| `.*` | Wskaźnik na składową | Jak wyżej |
| `sizeof` | Rozmiar typu/obiektu | Wynik musi być znany w czasie kompilacji |
| `typeid` | Informacja o typie | Część RTTI, działa na typach, nie wartościach |
| `?:` | Operator trójargumentowy | Nieznana liczba argumentów + short-circuit eval |

> **C++20** dodał `<=>` (operator „statku kosmicznego") jako precedens porównania trójstronnego —
> i ten **można** przeciążyć.

---

## Slajd 3: Których operatorów NIE NALEŻY przeciążać?

Nie wszystkie przeciążalne operatory *warto* przeciążać. Szczególnie ostrożnie z:

### Operatory `&&` i `||`

```cpp
// Dla typów wbudowanych: short-circuit evaluation
bool wynik = (ptr != nullptr) && ptr->isValid();
//                              ↑ to NIE wykona się, gdy ptr==nullptr

// Po przeciążeniu: short-circuit ZANIKA!
// operator&&(lhs, rhs) jest zwykłą funkcją
// — obie strony zostaną obliczone PRZED wywołaniem
```

### Operator `,`

```cpp
// Wbudowany: sekwencja wyrażeń, wartość ostatniego
int x = (a++, b++, c);   // zdefiniowana kolejność

// Przeciążony: kolejność argumentów przy wywołaniu
// funkcji jest nieokreślona → nieprzewidywalne zachowanie
```

### Operator `&` (jednoargumentowy adres)

```cpp
// Zwraca adres obiektu — przeciążenie dezorientuje użytkowników
// Biblioteki takie jak boost::spirit to robią, ale to wyjątki
```

> **Meyers (Effective C++, poz. 23):** Nie przeciążaj `&&`, `||` ani `,`.

---

## Slajd 4: Które operatory zawsze muszą być metodami?

Pięć operatorów **musi** być zdefiniowanych jako **metody klasy** (nie wolne funkcje):

| Operator | Wymóg | Uzasadnienie |
|----------|-------|-------------|
| `=` | Metoda | Przypisanie modyfikuje lewy operand |
| `[]` | Metoda | Wymagany dostęp do stanu obiektu |
| `()` | Metoda | Wywołanie funkcji/funktoru |
| `->` | Metoda | Dostęp przez wskaźnik do składowej |
| (konwersja) `operator T()` | Metoda | Konwertuje `this` na inny typ |

---

## Slajd 5: Tabela – metoda vs. wolna funkcja

| Sytuacja | Zalecana forma |
|----------|----------------|
| Operator modyfikuje lewy operand (`+=`, `++`) | **Metoda** |
| Lewy operand może być typem wbudowanym (`2 * Wektor`) | **Wolna funkcja** |
| Operator symetryczny (`+`, `==`, `<`) | **Wolna funkcja** (lub metoda, ale wolna lepsza) |
| Operator `<<`, `>>` (strumień po lewej) | **Wolna funkcja** (obowiązkowo) |
| Operator `=`, `[]`, `()`, `->` | **Metoda** (obowiązkowo) |

---

## Slajd 6: Diagram – mapa operatorów

![Diagram operatorów](operators_diagram.png)

<!-- Wygeneruj PNG z PlantUML: plantuml operators_diagram.puml -->

```
Operatory C++
├── Można przeciążyć
│   ├── Arytmetyczne     + - * / %
│   ├── Porównania       == != < > <= >=   (C++20: <=>)
│   ├── Logiczne         && || !           (ostrożnie!)
│   ├── Bitowe           & | ^ ~ << >>
│   ├── Przypisanie      = += -= *= /= …
│   ├── Inkrementacja    ++ --
│   ├── Dostęp           [] () -> ->*
│   ├── Alokacja         new new[] delete delete[]
│   └── I/O              << >>
└── NIE można przeciążyć
    └── :: . .* sizeof typeid ?:
```

---

## Slajd 7: Przykład – wpływ na integrację z STL

```cpp
#include <vector>
#include <algorithm>

struct Temp { double val; bool operator<(const Temp& o) const { return val < o.val; } };

std::vector<Temp> v = { {37.5}, {36.6}, {38.2} };
std::sort(v.begin(), v.end());  // działa dzięki operator<
```

Bez `operator<` wywołanie `std::sort` skończy się **błędem kompilacji**.

Podobnie `std::map` i `std::set` wymagają `operator<`,
a `std::cout` – `operator<<`.

---

## Slajd 8: Przykład kodu demonstracyjnego

Plik: [`src/main.cpp`](src/main.cpp)

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

struct Matrix2x2 {
    double a, b, c, d;
    Matrix2x2 operator+(const Matrix2x2& m) const {
        return {a+m.a, b+m.b, c+m.c, d+m.d};
    }
    Matrix2x2 operator*(const Matrix2x2& m) const {
        return { a*m.a+b*m.c, a*m.b+b*m.d,
                 c*m.a+d*m.c, c*m.b+d*m.d };
    }
    Matrix2x2 operator-() const { return {-a,-b,-c,-d}; }
    bool operator==(const Matrix2x2& m) const {
        return a==m.a && b==m.b && c==m.c && d==m.d;
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        return os << "[" << m.a <<" "<< m.b <<" / "<< m.c <<" "<< m.d <<"]";
    }
};
```

---

## Slajd 9: Operatory obowiązkowo-metodowe – `[]`, `()`, konwersja

Te operatory **muszą** być metodami klasy (standard języka to wymusza):

### `operator[]` – indeksowanie

```cpp
class IntArray {
    int* data_;
    int  size_;
public:
    IntArray(int size) : size_(size), data_(new int[size]{}) {}
    ~IntArray() { delete[] data_; }

    // Wersja do zapisu i odczytu
    int& operator[](int idx) { return data_[idx]; }
    // Wersja tylko do odczytu (na const obiekcie)
    const int& operator[](int idx) const { return data_[idx]; }
};

IntArray arr(5);
arr[2] = 42;              // operator[] zwraca referencję → można przypisać
std::cout << arr[2];      // 42
```

### `operator()` – funktor (obiekt wywoływalny)

```cpp
struct Multiplier {
    double factor;
    double operator()(double x) const { return x * factor; }
};

Multiplier twice{2.0};
std::cout << twice(3.14);   // 6.28 — obiekt wywołany jak funkcja
// Przydatne w STL: std::transform(v.begin(), v.end(), v.begin(), twice);
```

### `operator T()` – konwersja typu

```cpp
class Celsius {
    double val_;
public:
    explicit Celsius(double v) : val_(v) {}
    // Konwersja Celsius → double
    explicit operator double() const { return val_; }
    // Konwersja Celsius → bool (czy niezerowa)
    explicit operator bool() const { return val_ != 0.0; }
};

Celsius t(36.6);
double d = static_cast<double>(t);   // 36.6 — explicit wymusza static_cast
if (t) { /* ... */ }                 // operator bool() — w if() działa nawet explicit
```

> **Meyers (Effective C++, poz. 27):** Używaj `explicit` w konwersjach – zapobiega zaskakującym
> konwersjom niejawnym.

---

## Slajd 10: Operator statku kosmicznego `<=>` (C++20)

Operator `<=>` (trójstronne porównanie) pozwala zastąpić sześć osobnych operatorów relacyjnych
(`<`, `>`, `<=`, `>=`, `==`, `!=`) jedną definicją.

### Typ zwracany

```cpp
#include <compare>
```

| Typ zwracany | Kiedy użyć | Przykład |
|---|---|---|
| `std::strong_ordering` | Pełny porządek, brak remisów | `int`, `long` |
| `std::weak_ordering` | Porządek, ale remisy możliwe | ciągi case-insensitive |
| `std::partial_ordering` | Niektóre pary nieporównywalne | `float` (NaN) |

### Przeciążenie – wersja domyślna (zalecana)

Użycie `= default` zleca kompilatorowi wygenerowanie operatora automatycznie.
Kompilator porównuje **pola w kolejności deklaracji**, leksykograficznie (jak w słowniku):
najpierw pierwsze pole, a jeśli są równe – następne, i tak dalej.

Typ zwracany jest dedukowany jako **najwęższy wspólny typ** spośród typów zwracanych przez `<=>`
wszystkich pól (np. jeśli jedno pole to `double`, wynik to `partial_ordering`).

```cpp
#include <compare>

struct Punkt {
    int x, y;

    // Kompilator generuje ==, !=, <, >, <=, >= na podstawie kolejności pól
    // Typ: strong_ordering (bo oba pola to int)
    auto operator<=>(const Punkt&) const = default;
};

Punkt a{1, 2}, b{1, 3};
// porównanie: najpierw x (1==1 → remis), potem y (2 < 3 → a < b)
bool mniej = (a < b);   // true

struct Prostokat {
    double szerokosc, wysokosc;

    // Typ: partial_ordering (bo pola to double – NaN jest nieporównywalne)
    auto operator<=>(const Prostokat&) const = default;
};
```

**Co generuje kompilator przy `= default`:**

| Wygenerowany operator | Na podstawie |
|---|---|
| `operator<=>` | porównanie pól po kolei |
| `operator==` | porównanie pól po kolei (osobno generowany) |
| `operator!=` | negacja `operator==` |
| `operator<` | wynik `<=>` < 0 |
| `operator>` | wynik `<=>` > 0 |
| `operator<=` | wynik `<=>` <= 0 |
| `operator>=` | wynik `<=>` >= 0 |

### Przeciążenie – wersja ręczna

```cpp
#include <compare>

struct Temperatura {
    double stopnie;

    std::partial_ordering operator<=>(const Temperatura& other) const {
        return stopnie <=> other.stopnie;  // partial_ordering bo double może być NaN
    }
    bool operator==(const Temperatura& other) const {
        return stopnie == other.stopnie;
    }
};
```

> **Uwaga:** przy ręcznej definicji `<=>` kompilator **nie generuje** `operator==` automatycznie —
> trzeba go zdefiniować osobno.

---

## Slajd 11: Short-circuit evaluation – dlaczego nie przeciążać `&&` i `||`

**Short-circuit evaluation** (ewaluacja leniwa) oznacza, że drugi operand jest obliczany
**tylko wtedy, gdy wynik nie jest jeszcze znany** po pierwszym.

### Wbudowane `&&` i `||`

```cpp
// &&: jeśli lewa strona == false → prawa NIE jest obliczana
bool wynik = (ptr != nullptr) && ptr->isValid();
//            ↑ false → zatrzymaj  ↑ bezpieczne, nie wykona się gdy ptr==nullptr

// ||: jeśli lewa strona == true → prawa NIE jest obliczana
bool wynik2 = (a == 0) || (b / a > 2);
//             ↑ true → zatrzymaj  ↑ bezpieczne, brak dzielenia przez zero
```

### Po przeciążeniu – short-circuit **zanika**

```cpp
struct Flaga {
    bool val;
    Flaga operator&&(const Flaga& rhs) const { return {val && rhs.val}; }
};

Flaga a{false}, b = oblicz_cokolwiek();
Flaga wynik = a && b;
// oblicz_cokolwiek() ZAWSZE się wywoła – przeciążony operator to zwykła funkcja,
// a przed jej wywołaniem oba argumenty muszą być obliczone
```

| | Typy wbudowane | Przeciążony `operator&&` |
|---|---|---|
| Kolejność obliczeń | lewa → prawa (leniwie) | obie strony **zawsze** |
| Efekty uboczne prawej strony | możliwe do pominięcia | **zawsze** wystąpią |
| Bezpieczeństwo z nullptr | tak | nie gwarantowane |

> **Meyers (Effective C++, poz. 23):** Nie przeciążaj `&&`, `||` ani `,`.

---

## Slajd 12: Operator przecinka `,` – gwarancja kolejności

Wbudowany operator `,` to **operator sekwencji**: oblicza lewy operand, odrzuca jego wartość,
oblicza prawy i zwraca jego wartość. Gwarantuje zdefiniowaną kolejność i punkt sekwencji.

### Wbudowany `,`

```cpp
int a = 0, b = 0, c = 5;
int x = (a++, b++, c);
// 1. a++ → a == 1
// 2. b++ → b == 1
// 3. wynik wyrażenia == c == 5 → x = 5

// Najczęstsze użycie – pętla for z wieloma zmiennymi:
for (int i = 0, j = 10; i < j; ++i, --j) { /* ... */ }
//                                    ↑ operator ,
```

### Po przeciążeniu – kolejność **zanika**

```cpp
struct Kolekcja {
    Kolekcja operator,(const Kolekcja& rhs) const { /* ... */ }
};

Kolekcja a = generuj(), b = generuj2();
Kolekcja wynik = (a, b);
// kolejność obliczania argumentów funkcji w C++ jest NIEOKREŚLONA –
// kompilator może najpierw wywołać generuj2(), potem generuj()
```

| | Wbudowany `,` | Przeciążony `operator,` |
|---|---|---|
| Kolejność obliczeń | lewa → prawa (**gwarantowana**) | **nieokreślona** |
| Punkt sekwencji | tak | nie |
| Wartość wyrażenia | wartość prawego operandu | zależy od implementacji |

---

## Podsumowanie

| Pytanie | Odpowiedź |
|---------|-----------|
| Ile operatorów można przeciążyć? | Prawie wszystkie – ok. 40 |
| Których NIE wolno? | `::` `.` `.*` `sizeof` `typeid` `?:` |
| Których NIE NALEŻY? | `&&` `\|\|` `,` (tracą ważne właściwości) |
| Które MUSZĄ być metodami? | `=` `[]` `()` `->` konwersje |

---

## Dobre praktyki i antywzorce

- **Dobra praktyka:** przeciążaj `<=>` w C++20 — kompilator automatycznie generuje wtedy `<`, `>`, `<=`, `>=`.
- **Dobra praktyka:** definiuj `operator<` jako wolną funkcję – ułatwia użycie z kontenerami STL.
- **Antywzorzec:** przeciążenie `operator&&` lub `operator||` – zanika short-circuit evaluation.
- **Antywzorzec:** klasa bez `operator<<` trudna do debugowania i testowania.

## Pliki źródłowe

| Plik | Opis |
|------|------|
| [`src/main.cpp`](src/main.cpp) | Demonstracja klasy Matrix2x2 z różnymi operatorami |
| [`operators_diagram.puml`](operators_diagram.puml) | Mapa operatorów w C++ |
