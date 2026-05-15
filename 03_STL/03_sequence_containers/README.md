# STL – Kontenery sekwencyjne

## Slajd 1: Przegląd kontenerów sekwencyjnych

Kontenery sekwencyjne przechowują elementy **w określonej kolejności** ustalonej przez programistę
(nie przez wartość klucza).

| Kontener | Pamięć | Dostęp | `push_back` | `push_front` | `insert` środek |
|---|---|---|---|---|---|
| `vector` | ciągła | O(1) | O(1) amort. | O(n) | O(n) |
| `deque` | segmenty | O(1) | O(1) amort. | O(1) amort. | O(n) |
| `list` | węzły | O(n) | O(1) | O(1) | O(1) |
| `forward_list` | węzły | O(n) | — | O(1) | O(1) |
| `array<T,N>` | ciągła | O(1) | — | — | — |

> **Zasada kciuka:** używaj `vector` dopóki nie masz powodu do czegoś innego.

---

## Slajd 2: `std::vector` — ciągła tablica dynamiczna

```cpp
#include <vector>

std::vector<int> v;             // pusty
std::vector<int> v2(5, 0);     // 5 zer
std::vector<int> v3 = {1, 2, 3, 4, 5};

// Dostęp
v3[2];          // 3 – brak sprawdzania zakresu
v3.at(2);       // 3 – rzuca std::out_of_range gdy out of bounds

// Dodawanie
v3.push_back(6);         // na koniec
v3.emplace_back(7);      // konstruuje w miejscu (bez kopii)

// Rozmiar i pojemność
v3.size();      // liczba elementów
v3.capacity();  // zarezerwowana pamięć
v3.reserve(20); // zarezerwuj miejsce na 20 (bez zmiany size)
v3.shrink_to_fit(); // zwolnij nadmiar pamięci

// Usuwanie
v3.pop_back();          // usuń ostatni
v3.erase(v3.begin());   // usuń pierwszy
v3.clear();             // usuń wszystko
```

---

## Slajd 3: Mechanizm podwajania — amortyzowany O(1)

Gdy `size == capacity`, `push_back` powoduje **realokację**:

```
Stan:  [1][2][3]    size=3, capacity=3
push_back(4):
  1. Alokuj nową tablicę capacity=6
  2. Skopiuj 1,2,3 do nowej tablicy  ← O(n) raz na n operacji
  3. Dodaj 4
  Wynik: [1][2][3][4][_][_]  size=4, capacity=6

push_back(5): O(1) – jest miejsce
push_back(6): O(1) – jest miejsce
push_back(7): znów realokacja → capacity=12
```

Całkowity koszt n operacji `push_back`:
$$ \underbrace{n}_{\text{same push\_back}} + \underbrace{1+2+4+\ldots+n}_{\text{kopie przy realokacjach}} = n + 2n = O(n) $$

Średni koszt jednej operacji: **O(1)** — stąd "amortyzowany O(1)".

> Jeśli znasz docelowy rozmiar, użyj `reserve()` — eliminuje wszystkie realokacje.

---

## Slajd 4: `std::deque` — kolejka dwustronna

```cpp
#include <deque>

std::deque<int> d = {2, 3, 4};

d.push_front(1);    // O(1) – dodaj na początek: [1,2,3,4]
d.push_back(5);     // O(1) – dodaj na koniec:  [1,2,3,4,5]
d.pop_front();      // O(1) – usuń z początku:  [2,3,4,5]
d.pop_back();       // O(1) – usuń z końca:     [2,3,4]

d[1];               // O(1) – dostęp przez indeks
```

Wewnętrznie `deque` przechowuje dane w **stałych blokach** (chunks) —
nie jest ciągła w pamięci jak `vector`. Dostęp O(1) jest możliwy dzięki
mapie wskaźników na bloki.

**Kiedy używać deque zamiast vector:**
- potrzebujesz O(1) `push_front` (kolejka, BFS)
- przechowujesz duże elementy i realokacja `vector` byłaby kosztowna

---

## Slajd 5: `std::list` — lista dwukierunkowa

```cpp
#include <list>

std::list<int> lst = {1, 2, 3, 4, 5};

// O(1) insert/erase w dowolnym miejscu (gdy mamy iterator)
auto it = std::find(lst.begin(), lst.end(), 3);
lst.insert(it, 99);     // wstaw 99 przed 3: [1,2,99,3,4,5]
lst.erase(it);          // usuń 3:           [1,2,99,4,5]

// Specjalne operacje listy
lst.sort();             // własna wersja sort (nie std::sort!)
lst.reverse();
lst.unique();           // usuń kolejne duplikaty
lst.splice(lst.begin(), lst2); // przenieś całą lst2 do lst (O(1)!)
```

**Kiedy warto:** częste wstawianie/usuwanie w środku przy stabilnych iteratorach.  
**Kiedy nie warto:** prawie zawsze — `vector` z `erase` jest często szybszy w praktyce
ze względu na lokalność pamięci (cache).

---

## Slajd 6: `std::array` — bezpieczna tablica stałego rozmiaru

```cpp
#include <array>

std::array<int, 5> a = {1, 2, 3, 4, 5};  // rozmiar jest częścią typu!

a[2];           // 3
a.at(2);        // 3 z kontrolą zakresu
a.size();       // 5 – constexpr, znany w czasie kompilacji
a.front();      // 1
a.back();       // 5
a.data();       // int* – wskaźnik na surową tablicę (kompatybilność z C)

// Pełna integracja z algorytmami STL:
std::sort(a.begin(), a.end());
std::fill(a.begin(), a.end(), 0);
```

Zalety nad tablicą C (`int arr[5]`):
- Nie gubi rozmiaru przy przekazaniu do funkcji
- Ma `begin()`/`end()`, działa z algorytmami STL
- Można kopiować przez `=`
- `at()` sprawdza zakres

---

## Slajd 7: Porównanie złożoności

| Operacja | `vector` | `deque` | `list` | `array` |
|---|---|---|---|---|
| Dostęp `[i]` | O(1) | O(1) | O(n) | O(1) |
| `push_back` | O(1)* | O(1)* | O(1) | — |
| `push_front` | O(n) | O(1)* | O(1) | — |
| `insert` środek | O(n) | O(n) | **O(1)†** | — |
| `erase` środek | O(n) | O(n) | **O(1)†** | — |
| `find` | O(n) | O(n) | O(n) | O(n) |
| Lokalność pamięci | **dobra** | średnia | zła | **dobra** |

\* amortyzowany  
† wymaga posiadania iteratora do miejsca

---

## Slajd 8: Drzewko decyzyjne — który kontener wybrać?

```
Czy rozmiar jest znany w czasie kompilacji?
│
├── TAK → std::array<T, N>
│
└── NIE
    │
    ├── Czy potrzebujesz szybkiego push_front?
    │   ├── TAK → std::deque
    │   └── NIE
    │       │
    │       ├── Czy często wstawiasz/usuwasz w środku
    │       │   i nie potrzebujesz dostępu przez indeks?
    │       │   ├── TAK → std::list (lub forward_list)
    │       │   └── NIE → std::vector  ← domyślny wybór
    │       │
    │       └── (specjalne: kolejka → queue/deque, stos → stack/vector)
```

---

## Pliki źródłowe

| Plik | Opis |
|------|------|
| [`src/main.cpp`](src/main.cpp) | Demonstracja wszystkich kontenerów sekwencyjnych |
| [`sequence_diagram.puml`](sequence_diagram.puml) | Schemat pamięci i porównanie kontenerów |
| [`sequence_diagram.png`](sequence_diagram.png) | Wygenerowany diagram PNG |

![Kontenery sekwencyjne – układ pamięci](sequence_diagram.png)
