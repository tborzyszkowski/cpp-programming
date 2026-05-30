# Szablony – Historia i Motywacja

![Diagram](history_diagram.png)

## Slajd 1: Życie przed szablonami – makra preprocesora

Przed C++98 jedynym sposobem na „generyczny" kod w C było użycie makr:

```c
/* C – generyczność przez makra (lata 70./80.) */
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while(0)

/* Użycie: */
int x = MAX(3, 5);          /* OK */
double y = MAX(1.5, 2.5);   /* OK */
int z = MAX(i++, j++);      /* BŁĄD: i i j inkrementowane DWUKROTNIE! */
```

**Problemy makr:**
- Brak sprawdzania typów — kompilator nie widzi semantyki
- Trudny debuging — stack trace wskazuje na makro, nie wywołanie
- Brak zasięgu — makra są globalne, łatwo o kolizje nazw
- Efekty uboczne przy wielokrotnej ewaluacji argumentów

**Dlaczego makra są tak niebezpieczne?** Preprocesor to prosty mechanizm podstawiania
tekstu — wykonuje się **przed** kompilatorem, bez żadnej wiedzy o typach czy semantyce
C++. `MAX(i++, j++)` rozwinnie się do `((i++) > (j++) ? (i++) : (j++))` — `i` lub `j`
zostanie zinkrementowane dwukrotnie, co jest **niezdefiniowanym zachowaniem**. Kompilator
widzi ten kod dopiero po podstawieniu — nie ma szans ostrzec o podwójnej inkrementacji.
To fundamentalna różnica: szablon `max<T>` jest *funkcją* — argumenty są obliczane raz,
przekazane przez wartość lub referencję. Bezpieczeństwo typów i jednokrotna ewaluacja są
gwarantowane przez reguły języka, a nie przez ostrożność programisty.

---

## Slajd 2: `void*` jako ersatz generyczności

Drugim podejściem było `void*` — technika z biblioteki C (qsort, bsearch):

```c
/* C-style "generyczny" stos przez void* */
struct Stack {
    void** data;
    int    size;
    int    capacity;
};

void stack_push(Stack* s, void* item) { s->data[s->size++] = item; }
void* stack_pop(Stack* s)             { return s->data[--s->size]; }

/* Użycie – wymaga rzutowań, brak bezpieczeństwa typów */
Stack s;
int val = 42;
stack_push(&s, &val);
int* result = (int*)stack_pop(&s);  /* Ręczne rzutowanie, UB gdy pomylisz typ */
```

```cpp
/* C++98 bez szablonów – to samo z klasami */
class IntStack {  /* Dla int */
    int* data; int size;
public:
    void push(int x);
    int  pop();
};

class DoubleStack {  /* Kopiuj–wklej dla double! */
    double* data; int size;
public:
    void push(double x);
    double pop();
};
/* Explosion: IntStack, DoubleStack, StringStack... */
```

**Koszt duplikacji kodu:** Technika `void*` przenosi problem z czasu kompilacji do czasu
wykonania. Każde rzutowanie `(int*)stack_pop(&s)` to zakład: programista obiecuje
kompilatorowi, że wie, co tam jest. Gdy pomyli typy — nie ma błędu kompilacji, jest
**cicha korupcja danych** lub crash. Biblioteka C (qsort, bsearch) pokazuje skutki:
`qsort` przyjmuje `void*` i wskaźnik na komparator — nie ma gwarancji, że komparator
pasuje do typów danych. W C++ ta technika jest uznana za antywzorzec; szablony rozwiązują
problem *bez żadnego narzutu runtime*, bo duplikacja odbywa się w czasie kompilacji
(każda instancja szablonu jest osobną, optymalnie skompilowaną funkcją).

---

## Slajd 3: Inspiracje – Ada, ML i programowanie generyczne

Koncepcja szablonów nie powstała w próżni. Kluczowe inspiracje:

| Język | Rok | Mechanizm |
|-------|-----|-----------|
| **Ada** | 1983 | `generic package Stack is ...` — jawne generyczne pakiety |
| **ML / SML** | 1973/1984 | Polimorfizm parametryczny — `'a list`, `'a -> 'b` |
| **Haskell** | 1990 | Klasy typów (*type classes*) — przyszłe concepts |
| **Eiffel** | 1985 | Generyczność z ograniczeniami (`G -> COMPARABLE`) |
| **C++** | **1991** | `template<typename T>` — Bjarne Stroustrup |

```
Ada:                         C++ (analogia):
generic                      template<typename T>
  type Item is private;      class Stack {
package Stack is               T* data;
  procedure Push(X: Item);   public:
  function Pop return Item;    void push(T x);
end Stack;                     T    pop();
                             };
```

Kluczowy wkład **Aleksandra Stepanova**: udowodnienie, że algorytmy generyczne
mogą być *tak samo szybkie* jak specjalizowane — co obaliło powszechne przekonanie.

**Rola Stepanova jest kluczowa:** Przed STL (C++98) panowało przekonanie, że generyczny
kod musi być wolniejszy od specjalizowanego. Stepanov w pracy w Bell Labs i Hewlett-Packard
(1986–1994) empirycznie udowodnił tezę odwrotną: szablony C++ pozwalają na pełną
monomorfizację — kompilator generuje kod tak dobry jak ręcznie napisany dla każdego typu.
Jego implementacja `std::sort` jest szybsza od qsort z C o 15–50%, bo qsort używa
wskaźnika na funkcję (pośrednie wywołanie), a `std::sort` z szablonem komparatora
może zinlinować porównanie całkowicie. To był przełom filozoficzny: abstrakcja bez kosztu.

---

## Slajd 4: C++98 – pierwsze szablony

Bjarne Stroustrup wprowadził szablony do C++ w 1991 roku; weszły do standardu C++98:

```cpp
// C++98: szablon funkcji
template<typename T>
T maksimum(T a, T b) {
    return a > b ? a : b;
}

// C++98: szablon klasy
template<typename T>
class Stos {
    std::vector<T> dane_;
public:
    void push(const T& x) { dane_.push_back(x); }
    T    pop()            { T x = dane_.back(); dane_.pop_back(); return x; }
    bool empty() const    { return dane_.empty(); }
};

// Użycie – kompilator generuje kod dla każdego T:
Stos<int>         si;
Stos<std::string> ss;
Stos<double>      sd;
// Trzy różne klasy wygenerowane w czasie kompilacji
```

**Rewolucja:** Jeden kod → wiele instancji → pełna optymalizacja dla każdego typu.

**Mechanizm instancjacji:** Gdy kompilator napotka `Stos<int>`, generuje osobną klasę
identyczną z ręcznie napisaną `class IntStack` — z dokładnie takimi samymi polami i
metodami, tylko z `int` zamiast `T`. Ten proces to **instancjacja szablonu** i odbywa się
wyłącznie w czasie kompilacji. W wynikowej binarce nie ma żadnego śladu szablonu jako
takiego — jest tylko konkretna klasa dla każdego użytego `T`. Stąd nazwa „Zero-overhead
abstractions": generyczny kod jest równie szybki jak specjalizowany, bo po kompilacji
jest tym samym kodem. Koszt to czas kompilacji i rozmiar binarki przy wielu instancjach.

---

## Slajd 5: Problemy wczesnych szablonów – błędy i SFINAE

Szablony C++98 miały poważne słabości:

```cpp
// Problem 1: Błędy kompilacji były STRASZNE
template<typename T>
void sortuj(T& kol) {
    std::sort(kol.begin(), kol.end());
}

sortuj(42);  /* BŁĄD – komunikat zajmuje 50 linii! */
/* Kompilator mówi o brakującym begin() na int,
   a nie o tym, że int nie jest kontenerem */

// Problem 2: Brak możliwości odrzucenia niepasujących typów
template<typename T>
T policz(T a, T b) { return a + b; }
// Kompilator przyjmie int*, std::string*, cokolwiek...
// Błąd dopiero przy instancjacji
```

Rozwiązaniem było **SFINAE** (Substitution Failure Is Not An Error) — technika z 1998 r.:

```cpp
// SFINAE (C++98/C++11) – warunkowe włączanie przeciążeń
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
dodaj(T a, T b) { return a + b; }
// Ta funkcja istnieje TYLKO gdy T jest typem arytmetycznym
// Dla std::string – nie istnieje (SFINAE, nie błąd)
```

**Jak działa SFINAE?** Gdy kompilator próbuje podstawić `T = std::string` do szablonu
z `std::enable_if<std::is_arithmetic<T>::value, T>::type`, wyrażenie to jest **niepoprawne**
(bo `is_arithmetic<string>::value` jest `false`). Zamiast błędu kompilacji, kompilator
po prostu **pomija** tę przeciążoną wersję i szuka dalej. Jeśli nie znajdzie żadnego
pasującego przeciążenia — wtedy dopiero błąd. Zasada: niepowodzenie przy podstawianiu
do sygnatury to nie błąd — `Substitution Failure Is Not An Error`. SFINAE jest potężne,
ale składnia `enable_if` jest czytelna tylko dla zaawansowanych. C++20 Concepts zastąpiły
tę technikę czytelną składnią `requires`, zachowując ten sam model semantyczny.

---

## Slajd 6: Ewolucja szablonów – C++11 do C++20

| Standard | Nowości szablonów |
|----------|-------------------|
| **C++98** | Szablony funkcji i klas, specjalizacja, explicit instantiation |
| **C++03** | Poprawki, brak nowych funkcji |
| **C++11** | Variadic templates `<Ts...>`, `auto` return, `decltype`, `nullptr`, rvalue refs, extern templates |
| **C++14** | Generic lambdas, variable templates `template<T> constexpr T pi = ...` |
| **C++17** | Class template argument deduction (CTAD), `if constexpr`, fold expressions, `std::void_t` |
| **C++20** | **Concepts** (`concept`, `requires`), abbreviated function templates (`auto` params), `consteval` |
| **C++23** | `if consteval`, explicit `this`, dedukcja szablonu dla aliasów |

```
Makra (C)  →  void* (C)  →  Templates C++98  →  SFINAE C++11  →  Concepts C++20
  ↓               ↓              ↓                   ↓                  ↓
Brak typów    Brak typów     Niskie błędy       Czytelniejsze     Jasne kontrakty
              Niebezpieczne  Niejawny kontrakt  ale nadal złożone  Piękne błędy
```

**Ewolucja jest gradientem bezpieczeństwa i czytelności.** Każdy krok poprawiał albo
bezpieczeństwo (makra → szablony), albo czytelność błędów (szablony → SFINAE → Concepts).
Żaden krok nie popsuł wydajności — każde podejście generuje ten sam kod maszynowy dla
happy path. Kluczowy insight: problemem szablonów pre-C++20 nie była wydajność, lecz
**komunikacja**: błąd w szablonie ujawniał się 3 poziomy głębiej niż miejsce użycia,
z komunikatem opisującym szczegóły implementacji zamiast naruszenia kontraktu.

---

## Slajd 7: Concepts – idea zanim powstały

Koncepcja „concepts" istniała jako pomysł od lat 90. i była planowana na C++0x:

- **1994** – Stepanov w STL używa nieformalnych „requirements" (InputIterator, Sortable...)
- **2003** – propozycja `concept` do C++0x (Douglas Gregor, Jeremy Siek, Bjarne Stroustrup)
- **2009** – propozycja odrzucona — zbyt skomplikowana
- **2011** – C++11 bez concepts; w zamian SFINAE i type traits
- **2013** – Concepts Lite — uproszczona propozycja (Andrew Sutton)
- **2020** – **Concepts wchodzą do standardu jako część C++20**

```cpp
// Nieformalny „concept" z dokumentacji STL C++98:
// template<typename T>
// T max(T a, T b);
// Wymagania (napisane w komentarzu!):
//   T musi być LessThanComparable
//   T musi być CopyConstructible

// C++20 – to samo formalnie:
template<std::totally_ordered T>
T max(T a, T b) { return a > b ? a : b; }
// Kompilator WYMUSI te wymagania, nie tylko dokumentacja!
```

**Kontrakt formalny vs. dokumentacyjny — krytyczna różnica.** Komentarz napisany przez
programistę jest czytany przez człowieka, ale ignorowany przez kompilator. Jeśli ktoś
wywoła pre-C++20 `max<MyType>` dla typu bez `operator<`, błąd pojawi się w **środku
implementacji** `max`, kilka linii od `a > b`. W C++20 z `std::totally_ordered` błąd
pojawia się w **miejscu wywołania**: „typ MyType nie spełnia wymagania totally_ordered".
To zasadnicza zmiana: naruszenie kontraktu jest wykrywalne przez narzędzia (IDE,
kompilator) a nie tylko przez przeczytanie dokumentacji. Concepts są formalizacją tego,
co przez 20 lat było konwencją — szablony STL zawsze miały wymagania, ale zakodowane
tylko w komentarzach i niejasnych błędach kompilacji.
