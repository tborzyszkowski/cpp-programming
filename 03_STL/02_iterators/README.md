# STL – Iteratory

## Slajd 1: Czym jest iterator

Iterator to **uogólniony wskaźnik** — obiekt wskazujący na element zakresu,
który można przesuwać i wyłuskiwać. Jest mostem między kontenerem a algorytmem.

```cpp
std::vector<int> v = {10, 20, 30, 40};

std::vector<int>::iterator it = v.begin();  // wskazuje na 10
std::cout << *it;   // 10 – wyłuskanie
++it;               // przesuń na kolejny element
std::cout << *it;   // 20

// Pętla ręczna przez iteratory:
for (auto it = v.begin(); it != v.end(); ++it)
    std::cout << *it << " ";
```

Koniec zakresu (`end()`) wskazuje **za** ostatnim elementem — nigdy go nie wyłuskujemy.
Zakres `[begin, end)` to konwencja STL: lewostronnie domknięty, prawostronnie otwarty.

**Dlaczego zakres `[begin, end)` jest otwarty po prawej stronie?** Ta konwencja ma
kilka matematycznych i praktycznych zalet. Po pierwsze, dla pustego kontenera
`begin() == end()` — jeden warunek obsługuje przypadek brzegowy. Po drugie, liczba
elementów to po prostu `end - begin` (dla iteratorów random access). Po trzecie, pętle
`for (it = begin; it != end; ++it)` działają poprawnie dla dowolnej liczby elementów,
w tym zero. Alternatywa — zakres domknięty `[begin, last]` — wymagałaby specjalnego
reprezentowania pustego zakresu i komplikowała arytmetykę. Konwencja ta jest spójna
z matematyczną notacją przedziałów i stosowana konsekwentnie w całej STL.

---

## Slajd 2: Hierarchia kategorii iteratorów

Kategorie tworzą hierarchię zdolności — każda wyższa zawiera operacje niższych:

```
InputIterator ──────┐
                    ├──▶ ForwardIterator ──▶ BidirectionalIterator ──▶ RandomAccessIterator ──▶ ContiguousIterator
OutputIterator ─────┘
```

| Kategoria | Operacje | Przykład kontenera |
|---|---|---|
| **Input** | `*` (odczyt), `++`, `==` | `istream_iterator` |
| **Output** | `*` (zapis), `++` | `ostream_iterator` |
| **Forward** | Input + wielokrotne przejście | `forward_list` |
| **Bidirectional** | Forward + `--` | `list`, `map`, `set` |
| **Random Access** | Bidirectional + `+n`, `-n`, `[]`, `<` | `vector`, `deque` |
| **Contiguous** | Random Access + ciągła pamięć | `vector`, `array`, `string` |

**Hierarchia kategorii a wybór algorytmu.** Każda wyższa kategoria zawiera wszystkie
operacje niższych — `RandomAccessIterator` jest też `BidirectionalIterator`. Algorytmy
wymagają **minimalnej** kategorii: `std::sort` wymaga random access (bo potrzebuje
`it += n`), więc nie zadziała na `std::list`. `std::find` wymaga tylko input iterator,
więc działa wszędzie. Ta hierarchia to system typów dla iteratorów — przed C++20
kategoria była zakodowana w `iterator_category` (tag dispatching), w C++20 zastępuje
ją bardziej precyzyjny system concepts (`std::random_access_iterator<T>`). Praktyczna
implikacja: gdy przekazujesz `std::list` tam gdzie oczekiwany jest random access, błąd
kompilacji wskazuje na niezgodność kategorii iteratora.

---

## Slajd 3: Operacje na iteratorach

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin();

// Wyłuskanie
int val = *it;          // 1

// Przesunięcie
++it;                   // następny element
--it;                   // poprzedni (Bidirectional+)
it += 3;                // skok o 3 (RandomAccess+)
it -= 1;                // cofnięcie o 1 (RandomAccess+)

// Indeksowanie
int x = it[2];          // jak *(it + 2) (RandomAccess+)

// Odległość
auto dist = std::distance(v.begin(), v.end());  // 5

// Przesunięcie o n
auto it2 = std::next(it, 2);    // it + 2 (działa dla każdej kategorii)
auto it3 = std::prev(it, 1);    // it - 1 (Bidirectional+)
std::advance(it, 3);            // przesuń it o 3 w miejscu
```

**`std::next`, `std::prev`, `std::advance` — dlaczego istnieją?** Bezpośredni `it += n`
działa tylko dla random access iterators — dla `std::list` to błąd kompilacji. `std::advance(it, n)`
działa dla **każdej kategorii**: dla random access to O(1) (kompilator wybiera `+=`),
dla bidirectional z ujemnym n to O(|n|) powtórzeń `--`, dla forward zawsze O(n) `++`.
Dispatch odbywa się w czasie kompilacji na podstawie `iterator_category`. `std::distance`
analogicznie: O(1) dla random access (odejmowanie), O(n) dla pozostałych (liczenie kroków).
Używanie tych funkcji zamiast bezpośredniej arytmetyki czyni kod **generycznym** — działa
poprawnie dla każdego kontenera, nawet jeśli później zmienisz `vector` na `list`.

---

## Slajd 4: Rodzaje zakresów — `begin`, `end`, warianty

```cpp
std::vector<int> v = {1, 2, 3};

// Standardowe (odczyt/zapis)
v.begin()   v.end()

// Stałe – tylko do odczytu
v.cbegin()  v.cend()

// Odwrotne – iteracja od końca
v.rbegin()  v.rend()   // rbegin() wskazuje ostatni element

// Stałe odwrotne
v.crbegin() v.crend()

// Wolne funkcje (działają też na tablicach C):
std::begin(v)   std::end(v)

int arr[] = {4, 5, 6};
for (auto it = std::begin(arr); it != std::end(arr); ++it)
    std::cout << *it;
```

**Wolne funkcje `std::begin`/`std::end` — klucz do generyczności.** Metody `.begin()`
i `.end()` działają tylko na obiektach klas (kontenery STL). Tablice C (`int arr[5]`)
nie mają metod, ale mają wolne funkcje `std::begin(arr)`, `std::end(arr)`. Range-based
for używa właśnie wolnych funkcji — dlatego działa zarówno dla `std::vector`, jak i dla
zwykłej tablicy C. Możesz zdefiniować `begin()` i `end()` dla własnego typu jako wolne
funkcje w tej samej przestrzeni nazw (ADL) — range-based for automatycznie je znajdzie.
C++20 `std::ranges::begin` jest jeszcze bardziej niezawodne: sprawdza metody, potem
wolne funkcje, obsługuje tablice — jedno wywołanie dla wszystkich zakresów.

---

## Slajd 5: Range-based for — cukier składniowy

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Zapis skrócony:
for (int x : v) std::cout << x;

// Kompilator rozwija do:
{
    auto __begin = v.begin();
    auto __end   = v.end();
    for (; __begin != __end; ++__begin) {
        int x = *__begin;
        std::cout << x;
    }
}

// Modyfikacja przez referencję:
for (int& x : v) x *= 2;

// Unikanie kopii przez const&:
for (const auto& x : v) std::cout << x;
```

**Pułapki range-based for.** Trzy częste błędy: (1) pominięcie `&` przy dużych typach
— `for (auto x : vecStringow)` kopiuje każdy string; pisz `for (const auto& x : ...)`.
(2) Modyfikacja kontenera podczas iteracji unieważnia iteratory (niezdefiniowane
zachowanie). (3) `for (auto x : v)` dla `vector<bool>` zwraca proxy zamiast `bool&` —
`vector<bool>` to specjalizacja używająca bitów, nie zwykłych wartości. Dobra zasada:
zawsze używaj `const auto&` dla kontenerów z nietrywialnym typem elementów, `auto`
dla prostych typów liczbowych, `auto&` gdy chcesz modyfikować.

---

## Slajd 6: Iteratory unieważnione (dangling iterators)

Niektóre operacje **unieważniają** istniejące iteratory — użycie ich po unieważnieniu
to **niezdefiniowane zachowanie**.

```cpp
std::vector<int> v = {1, 2, 3};
auto it = v.begin();    // wskazuje na 1

v.push_back(4);         // ← może realokować pamięć!
// it jest teraz UNIEWAŻNIONY – nie wolno go używać

std::cout << *it;       // UB!
```

| Kontener | Operacja | Co unieważnia |
|---|---|---|
| `vector` | `push_back`, `insert`, `erase` | Wszystkie iteratory (jeśli realokacja) |
| `vector` | `erase` bez realokacji | Iteratory od miejsca usunięcia do końca |
| `deque` | `push_front/back` | Wszystkie iteratory |
| `list`, `map`, `set` | `insert` | Nic |
| `list`, `map`, `set` | `erase` | Tylko iterator na usuniętym elemencie |

**Unieważnienie iteratorów — najczęstsze źródło UB w STL.** Gdy `vector` realokuje
pamięć (bo `push_back` przekracza capacity), **wszystkie** istniejące iteratory,
wskaźniki i referencje stają się wiszące — wskazują na zwolnioną pamięć. Narzędzie
**AddressSanitizer** (kompilacja z `-fsanitize=address`) wykrywa takie błędy w runtime.
Bezpieczne wzorce: zapisuj wynik `erase` zamiast używać starego iteratora; jeśli
potrzebujesz stabilnych iteratorów przez wiele insertów — używaj `list` lub `map`.
`reserve()` dla `vector` przed serią `push_back` nie tylko przyspiesza kod, ale też
gwarantuje, że istniejące iteratory pozostaną ważne — bo nie ma realokacji.

---

## Slajd 7: C++20 Ranges — nowoczesna alternatywa

C++20 wprowadza `std::ranges::` — algorytmy przyjmują cały kontener zamiast pary iteratorów:

```cpp
#include <ranges>
#include <algorithm>

std::vector<int> v = {5, 3, 1, 4, 2};

// Stary styl (C++98):
std::sort(v.begin(), v.end());

// Nowy styl (C++20):
std::ranges::sort(v);

// Filtrowanie i transformacja (pipelines):
auto wynik = v
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; });

for (int x : wynik) std::cout << x << " ";  // kwadraty parzystych
```

Ranges są **leniwe** — `views::filter` i `views::transform` nie tworzą kopii,
obliczenia są wykonywane dopiero przy iteracji.

**Ranges zmieniają sposób myślenia o algorytmach.** Zamiast `std::sort(v.begin(), v.end())`
piszemy `std::ranges::sort(v)` — krótsze i mniej podatne na błąd podania złych iteratorów.
Ale prawdziwa siła to **kompozycja przez `|`**: każdy `view` to leniwa transformacja,
a połączenie kilku tworzy pipeline obliczany element po elemencie, bez tymczasowych
kolekcji. Odpowiednik `v | filter | transform` tradycyjnie wymagał dwóch tymczasowych
`vector` lub skomplikowanej lambdy z `copy_if` + `transform`. Views nie kopiują danych
— `views::filter` tworzy obiekt przechowujący referencję do zakresu i predykat;
element jest obliczany dopiero gdy iterator views zostanie wyłuskany. To model
analogiczny do generatorów w Pythonie.

---

## Pliki źródłowe

| Plik | Opis |
|------|------|
| [`src/main.cpp`](src/main.cpp) | Demonstracja kategorii iteratorów, unieważniania, Ranges |
| [`iterators_diagram.puml`](iterators_diagram.puml) | Hierarchia kategorii iteratorów |
| [`iterators_diagram.png`](iterators_diagram.png) | Wygenerowany diagram PNG |

![Hierarchia iteratorów](iterators_diagram.png)
