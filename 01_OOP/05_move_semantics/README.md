# Move Semantics i Konstruktor Przenoszący (C++11)

## Slajd 1: Problem z kopiowaniem

Przed C++11 przekazywanie i zwracanie obiektów zawierających zasoby dynamiczne wiązało się z **kosztownym kopiowaniem**:

```cpp
std::vector<Buffer> v;
v.push_back(Buffer("data", 1000000));   // przed C++11: głęboka kopia!
```

Kopiowanie dużego bufora = alokacja + `memcpy` + zwolnienie oryginału → marnowanie czasu i pamięci.

---

## Slajd 2: Referencje l-value vs r-value

```cpp
int x = 42;        // x jest l-value (ma adres, można przypisać)
int y = x + 1;     // x+1 jest r-value (tymczasowe, bez adresu)

Buffer b1("A", 5); // l-value (nazwany obiekt)
Buffer("B", 5);    // r-value (tymczasowy obiekt – nie ma nazwy!)
```

| Typ           | Opis                          | Referencja |
|---------------|-------------------------------|:----------:|
| l-value       | Trwały obiekt z adresem       | `T&`       |
| r-value       | Tymczasowy, bez nazwy         | `T&&`      |

---

## Slajd 3: Konstruktor przenoszący

```cpp
// Konstruktor KOPIUJĄCY – tworzy nowy blok
Buffer(const Buffer& other)
    : name_(other.name_ + "_copy"), size_(other.size_),
      data_(new int[other.size_]) {          // NOWA alokacja
    std::memcpy(data_, other.data_, ...);    // kopiowanie danych
}

// Konstruktor PRZENOSZĄCY – przejmuje blok, brak alokacji
Buffer(Buffer&& other) noexcept
    : name_(std::move(other.name_)),
      size_(other.size_),
      data_(other.data_) {         // PRZEJĘCIE wskaźnika
    other.data_ = nullptr;         // unieważnij źródło!
    other.size_ = 0;
}
```

Move jest tani: tylko przestawienie wskaźnika – O(1) zamiast O(n)!

---

## Slajd 4: `std::move` – jawne żądanie przeniesienia

`std::move` **nie przenosi** – jedynie *rzutuje* obiekt na r-value reference, co pozwala wybrać przeciążenie `&&`:

```cpp
Buffer b1("alpha", 5);
Buffer b3 = std::move(b1);   // jawnie żądamy move zamiast copy

b3.print();   // b3 ma dane
b1.print();   // b1: "moved-from" – data_ == nullptr, pusty!

// UWAGA: po std::move obiekt b1 jest w "valid but unspecified state"
// Nie używaj go bez ponownej inicjalizacji!
```

---

## Slajd 5: Move assignment operator

```cpp
Buffer& operator=(Buffer&& other) noexcept {
    if (this == &other) return *this;
    delete[] data_;              // zwolnij WŁASNY zasób
    name_ = std::move(other.name_);
    size_ = other.size_;
    data_ = other.data_;         // przejmij pointer
    other.data_ = nullptr;
    other.size_ = 0;
    return *this;
}

Buffer b4("beta", 3), b5("gamma", 3);
b4 = std::move(b5);   // move assignment
```

---

## Slajd 6: `noexcept` – dlaczego ważne?

```cpp
Buffer(Buffer&& other) noexcept { ... }      // ← noexcept obowiązkowy!
Buffer& operator=(Buffer&&) noexcept { ... }
```

`std::vector` przy realokacji **używa move tylko jeśli** konstruktor przenoszący jest `noexcept`.  
Bez `noexcept` – vector używa kopiowania (bezpiecznej opcji). Zawsze deklaruj `noexcept` na move!

---

## Slajd 7: Reguła Pięciu (C++11)

```
Jeśli zarządzasz zasobem, zdefiniuj WSZYSTKIE pięć:

1. ~Buffer()                              destruktor
2. Buffer(const Buffer&)                  konstruktor kopiujący
3. Buffer& operator=(const Buffer&)       operator= kopiujący
4. Buffer(Buffer&&) noexcept              konstruktor przenoszący  ← nowy C++11
5. Buffer& operator=(Buffer&&) noexcept   operator= przenoszący   ← nowy C++11
```

Alternatywa: `= default` / `= delete` zamiast ręcznej implementacji.

---

## Slajd 8: Diagram

![Diagram](move_diagram.puml)

```
KOPIA:
  b1.data_ ──► [ 0,1,2,3,4 ]  (blok A)
  b2.data_ ──► [ 0,1,2,3,4 ]  (blok B – nowa alokacja)

MOVE:
  b1.data_ ──► nullptr (moved-from)
  b3.data_ ──► [ 0,1,2,3,4 ]  (blok A – ten sam blok!)
```

---

## Slajd 9: Demonstracja – std::vector

Plik: [`src/main.cpp`](src/main.cpp)

```cpp
std::vector<Buffer> buffers;
buffers.reserve(3);   // unika realokacji (i dodatkowych move)

buffers.push_back(Buffer("v1", 4));   // Buffer("v1",4) to r-value → MOVE
buffers.push_back(Buffer("v2", 4));   // analogicznie
// Brak zbędnych kopii! Konstruktor przenoszący zamiast kopiującego.
```

---

## Slajd 10: Kompilacja i uruchomienie

```bash
g++ -std=c++17 -o move src/main.cpp
./move
```

Obserwuj komunikaty `[Buffer] MOVE ctor` – to potwierdza że przenoszenie działa.

---

## Podsumowanie

| Pojęcie                | Opis                                              |
|------------------------|---------------------------------------------------|
| l-value                | Nazwany obiekt z trwałym adresem                  |
| r-value                | Tymczasowy obiekt (bez nazwy)                     |
| `T&&`                  | Referencja r-value (r-value reference)            |
| Move ctor              | Przejmuje zasoby zamiast kopiować                 |
| `std::move(x)`         | Rzutuje x na r-value – umożliwia move             |
| `noexcept`             | Sygnalizuje że move nie rzuca wyjątków            |
| Reguła Pięciu          | 5 operacji specjalnych gdy zarządzamy zasobem     |

---

## Pliki źródłowe

| Plik                              | Opis                           |
|-----------------------------------|--------------------------------|
| [`src/Buffer.h`](src/Buffer.h)   | Klasa z copy i move operacjami |
| [`src/main.cpp`](src/main.cpp)   | Demonstracja move semantics    |
| [`move_diagram.puml`](move_diagram.puml) | Diagram UML             |
