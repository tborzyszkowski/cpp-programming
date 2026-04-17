# Stos i Sterta – Zarządzanie Pamięcią w C++ (Brak GC)

## Slajd 1: C++ nie ma Garbage Collectora!

W językach takich jak Java, Python, C# **Garbage Collector** automatycznie zwalnia nieużywaną pamięć.

**C++ nie ma GC!** Programista jest odpowiedzialny za każdą alokowaną pamięć:

```
Java/Python/C#:    new Foo() → GC zwolni kiedy niepotrzebne
C++:               new Foo() → TY musisz wywołać delete!
```

W zamian C++ daje pełną kontrolę i przewidywalną wydajność.

---

## Slajd 2: Stos (Stack) – automatyczna pamięć

**Stos (stack)** to obszar pamięci zarządzany automatycznie przez kompilator:

```cpp
void example() {
    int    x = 42;                // na stosie
    double d = 3.14;              // na stosie
    Point3D p(1, 1.0, 2.0, 3.0); // na stosie

    {
        Point3D inner(2, 4.0, 5.0, 6.0);  // na stosie (wewnętrzny blok)
    }   // ← ~dtor inner: AUTOMATYCZNIE

    // p nadal żyje
}   // ← ~dtor p, x, d: AUTOMATYCZNIE (LIFO)
```

Cechy stosu:
- Szybka alokacja – inkrementacja wskaźnika stosu
- Ograniczony rozmiar (domyślnie ~1–8 MB)
- Obiekty żyją **do końca bloku** w którym powstały
- Destruktory wywoływane **automatycznie** (LIFO)

---

## Slajd 3: Sterta (Heap) – dynamiczna pamięć

**Sterta (heap)** to duży obszar pamięci zarządzany przez `new`/`delete`:

```cpp
// Alokacja – konstruktor wywołany
Point3D* p = new Point3D(10, 1.0, 2.0, 3.0);

// Użycie przez wskaźnik
p->x = 99.0;
std::cout << p->toString() << "\n";

// OBOWIĄZKOWE zwolnienie – destruktor wywołany
delete p;
p = nullptr;   // dobra praktyka: usuń dangling pointer
```

Cechy sterty:
- Duży rozmiar (ograniczony przez RAM + swap)
- Obiekt żyje **do wywołania `delete`** lub końca programu
- Destruktor wywołany **tylko przez `delete`**
- Bez `delete` → **wyciek pamięci!**

---

## Slajd 4: Tablice dynamiczne

```cpp
// Alokacja tablicy
const int N = 5;
int* arr = new int[N];          // alokacja N intów
Point3D* pts = new Point3D[N];  // NxPoint3D – ctor dla każdego!

// Użycie
for (int i = 0; i < N; ++i)
    arr[i] = i * 2;

// Zwolnienie – WAŻNE: delete[] (z nawiasami!) dla tablic
delete[] arr;     // ✅ zwolnij tablicę
delete[] pts;     // ✅ destruktor dla każdego elementu!

// delete arr;    // ❌ BŁĄD: dla tablicy musi być delete[]!
```

---

## Slajd 5: Wyciek pamięci (Memory Leak)

```cpp
void badFunction() {
    Point3D* p = new Point3D(99);  // alokacja
    // ... jakaś praca ...
    // Brak delete p; → WYCIEK PAMIĘCI!
}   // p (wskaźnik stos) zniszczony, ale obiekt na stercie pozostaje!

// Efekt po wielu wywołaniach:
for (int i = 0; i < 1000000; ++i)
    badFunction();    // → program zjada coraz więcej RAM!
```

Wyciek pamięci w C++:
- Nie powoduje natychmiastowego błędu
- Program działa wolniej, zużywa więcej RAM
- Może zakończyć program przez `std::bad_alloc`
- **Valgrind** / **AddressSanitizer** wykrywają wycieki

---

## Slajd 6: RAII – idiom bezpiecznego zarządzania

**RAII** (Resource Acquisition Is Initialization) – zasada C++:

> Zasób jest przydzielany w konstruktorze, zwalniany w destruktorze.

```cpp
class ResourceSafeDemo {
    int* data_;
public:
    // Alokacja W KONSTRUKTORZE
    ResourceSafeDemo(int size) : data_(new int[size]) {}

    // Zwolnienie W DESTRUKTORZE – automatyczne, zawsze!
    ~ResourceSafeDemo() { delete[] data_; }
};

{
    ResourceSafeDemo safe(1000);   // ctor: alokacja
    // ... używaj ...
}   // ← dtor: zwolnienie AUTOMATYCZNIE (nawet przy wyjątku!)
```

---

## Slajd 7: Smart Pointers (C++11) – RAII gotowe rozwiązania

Smart pointer to obiekt-opakowanie na surowy wskaźnik. Dzięki RAII wywołuje `delete` automatycznie w swoim destruktorze – nawet gdy funkcja zakończy się wyjątkiem.

---

### `unique_ptr` – wyłączna własność

Jeden `unique_ptr` = jeden właściciel zasobu. Nie można go kopiować – tylko **przenosić** (`std::move`).  
To domyślny wybór: zerowy narzut pamięciowy i wydajnościowy względem gołego wskaźnika.

```cpp
#include <memory>

// Tworzenie – zawsze przez make_unique (nigdy new bezpośrednio)
auto p = std::make_unique<Point3D>(20, 5.0, 6.0, 7.0);

std::cout << p->toString() << "\n";   // operator-> jak na surowym wskaźniku
std::cout << (*p).toString() << "\n"; // lub operator*

// Transfer własności – po move p jest pusty (nullptr)
auto p2 = std::move(p);
// p  → nullptr (nie wolno używać!)
// p2 → ma obiekt

// auto p3 = p2;   // ❌ BŁĄD KOMPILACJI – kopiowanie zabronione

}   // ← destruktor p2: delete automatycznie, nawet przy wyjątku
```

**Kiedy używać:** wszędzie tam, gdzie jeden właściciel jest wystarczający – czyli w ~90% przypadków.

---

### `shared_ptr` – współdzielona własność

Wiele `shared_ptr` może wskazywać na ten sam obiekt. Wewnętrznie prowadzony jest **licznik referencji** (*reference count*). Obiekt jest niszczony gdy licznik spadnie do zera.

```cpp
auto sp1 = std::make_shared<Point3D>(21, 8.0, 9.0, 10.0);
// refcount = 1

{
    auto sp2 = sp1;          // kopiowanie → refcount = 2
    auto sp3 = sp1;          // kopiowanie → refcount = 3
    std::cout << sp1.use_count() << "\n";   // 3
}   // sp2, sp3 wypadają ze scope → refcount = 1

// sp1 nadal żyje
// refcount = 0 gdy sp1 wypadnie ze scope → delete
```

**Koszt:** `shared_ptr` przechowuje dodatkowy blok kontrolny (*control block*) z licznikiem – zajmuje więcej pamięci niż `unique_ptr` i operacje na liczniku są atomowe (thread-safe, ale wolniejsze).

**Kiedy używać:** gdy zasób musi być współdzielony przez wielu właścicieli (np. węzły grafu, cache, zdarzenia).

---

### `weak_ptr` – obserwator bez własności

`weak_ptr` wskazuje na obiekt zarządzany przez `shared_ptr`, ale **nie zwiększa licznika referencji**. Nie przedłuża życia obiektu.

Główne zastosowanie: **przerwanie cykli referencji**, które uniemożliwiłyby zwolnienie pamięci.

#### Problem cyklu – bez `weak_ptr`

```cpp
struct Node {
    std::shared_ptr<Node> next;   // silna referencja
    ~Node() { std::cout << "~Node\n"; }
};

auto a = std::make_shared<Node>();  // refcount(a) = 1
auto b = std::make_shared<Node>();  // refcount(b) = 1

a->next = b;   // refcount(b) = 2
b->next = a;   // refcount(a) = 2

// Koniec scope: a i b wychodzą → refcount spada do 1 (nie do 0!)
// Destruktory nigdy nie są wywoływane → WYCIEK PAMIĘCI!
```

```
a ──(shared)──► Node_A
                  │
               (shared)
                  ▼
               Node_B ──(shared)──► a   ← cykl!
```

#### Rozwiązanie – `weak_ptr`

```cpp
struct Node {
    std::weak_ptr<Node> next;     // słaba referencja – nie zwiększa licznika
    ~Node() { std::cout << "~Node\n"; }
};

auto a = std::make_shared<Node>();  // refcount(a) = 1
auto b = std::make_shared<Node>();  // refcount(b) = 1

a->next = b;   // refcount(b) nadal = 1  (weak nie liczy!)
b->next = a;   // refcount(a) nadal = 1

// Koniec scope: a → refcount = 0 → delete Node_A
//               b → refcount = 0 → delete Node_B
// Destruktory wywołane poprawnie!
```

#### Bezpieczne użycie `weak_ptr` – `lock()`

Przed dostępem do obiektu przez `weak_ptr` należy sprawdzić, czy obiekt jeszcze istnieje.  
Metoda `lock()` zwraca `shared_ptr` – jeśli obiekt już został zniszczony, zwraca `nullptr`.

```cpp
std::weak_ptr<Point3D> wp;

{
    auto sp = std::make_shared<Point3D>(1, 1.0, 2.0, 3.0);
    wp = sp;                        // wp obserwuje, nie jest właścicielem

    if (auto locked = wp.lock()) {  // lock() → shared_ptr lub nullptr
        std::cout << locked->toString() << "\n";   // ✅ obiekt żyje
    }
}   // sp wychodzi ze scope → obiekt zniszczony

if (auto locked = wp.lock()) {
    // ten blok się nie wykona
} else {
    std::cout << "Obiekt już nie istnieje\n";   // ✅
}

// wp.expired() → true gdy refcount == 0
```

---

### Porównanie smart pointerów

| Smart pointer  | Własność         | Licznik ref | Kopiowanie | Kiedy używać                             |
|----------------|------------------|:-----------:|:----------:|------------------------------------------|
| `unique_ptr`   | wyłączna         | brak        | ❌ (tylko move) | domyślnie – jeden właściciel         |
| `shared_ptr`   | współdzielona    | tak         | ✅          | wiele właścicieli                        |
| `weak_ptr`     | brak (obserwator)| nie liczy   | ✅          | obserwacja bez własności, łamanie cykli  |

**Zasada:** Preferuj smart pointery nad gołymi `new`/`delete`!  
Kolejność wyboru: `unique_ptr` → `shared_ptr` → `weak_ptr` (jako uzupełnienie `shared_ptr`).

---

## Slajd 8: Porównanie – stos vs. sterta

| Cecha               | Stos                        | Sterta                        |
|---------------------|-----------------------------|-------------------------------|
| Alokacja            | Automatyczna (bp + offset)  | `new`/`malloc`                |
| Zwolnienie          | Automatyczne (koniec bloku) | `delete`/`free` – RĘCZNIE    |
| Rozmiar             | Mały (~1-8 MB)              | Duży (RAM + swap)             |
| Szybkość            | Bardzo szybka               | Wolniejsza (zarządzanie)      |
| Czas życia          | Zakres bloku                | Do `delete` lub końca programu|
| Wyciek pamięci      | Niemożliwy                  | Możliwy (brak delete)         |
| Obiekt polimorficzny| Utrudniony                  | Naturalny (przez wskaźnik)    |

---

## Slajd 9: Diagram pamięci

![Diagram](memory_diagram.png)

```
┌──────────────────────────────────────────────────────────────┐
│                      PAMIĘĆ PROCESU                          │
├──────────────────┬───────────────────┬────────────────────── │
│     STOS         │     STERTA        │  SEGMENT STATYCZNY    │
│  (automatyczny)  │  (ręczny)         │  (zmienne statyczne)  │
│                  │                   │                        │
│ Point3D p1       │ Point3D* hp1 ──►  │ BankAccount::nextId_  │
│ Point3D p2       │ Point3D[3]   ──►  │ BankAccount::rate_    │
│ int x            │                   │                        │
│ [destruktory     │ [delete!          │                        │
│  auto przy LIFO] │  obowiązkowe]     │                        │
└──────────────────┴───────────────────┴────────────────────── ┘
```

---

## Slajd 10: Demonstracja kodu

Plik: [`src/main.cpp`](src/main.cpp)

```cpp
// Stos – automatyczne zarządzanie
void stackDemo() {
    Point3D p1(1, 1.0, 2.0, 3.0);   // ctor auto
    {
        Point3D p2(2, 4.0, 5.0, 6.0);
    }   // ← ~p2 automatycznie
}   // ← ~p1 automatycznie

// Sterta – ręczne zarządzanie
void heapDemo() {
    Point3D* p = new Point3D(10, 1.0, 2.0, 3.0);
    delete p;    // ← OBOWIĄZKOWE!
    p = nullptr; // ← zabezpieczenie

    int* arr = new int[5];
    delete[] arr;  // ← delete[] dla tablic!
}
```

---

## Slajd 11: Kompilacja i uruchomienie

```bash
g++ -std=c++17 -o memory src/main.cpp
./memory
```

Dla wykrycia wycieków (Linux):
```bash
g++ -std=c++17 -g -fsanitize=address -o memory src/main.cpp
./memory
```

---

## Podsumowanie

| Pojęcie              | Opis                                                   |
|----------------------|--------------------------------------------------------|
| Stos (stack)         | Automatyczna pamięć lokalna, szybka, ograniczona       |
| Sterta (heap)        | Dynamiczna pamięć, `new`/`delete`, duża               |
| `new`/`delete`       | Alokacja i zwalnianie na stercie                       |
| `new[]`/`delete[]`   | Tablice na stercie                                     |
| Memory leak          | Brak `delete` → wyciek pamięci                        |
| RAII                 | Zasoby zarządzane przez konstruktor/destruktor         |
| Smart pointers       | `unique_ptr`, `shared_ptr` – automatyczne zarządzanie |
| Brak GC              | C++ wymaga ręcznego zarządzania lub RAII               |

---

## Dobre praktyki, antywzorce i zastosowania

- Dobra praktyka: preferuj obiekty na stosie i RAII, a `new/delete` stosuj tylko gdy to konieczne.
- Dobra praktyka: dla dynamicznej pamięci wybieraj `std::unique_ptr` jako domyślną opcję.
- Dobra praktyka: zawsze paruj `new[]` z `delete[]` i `new` z `delete`.
- Antywzorzec: surowe wskaźniki jako właściciele zasobów bez jasnej odpowiedzialności za zwolnienie.
- Antywzorzec: "zapomniane" `delete`, które powoduje ciche wycieki i degradację wydajności.
- Zastosowanie: systemy embedded, silniki gier, komponenty czasu rzeczywistego i biblioteki C++.
- Zastosowanie: swiadome zarzadzanie pamiecia w kodzie wydajnosciowym i niskopoziomowym.

## Pliki źródłowe

| Plik                                  | Opis                              |
|---------------------------------------|-----------------------------------|
| [`src/MemoryDemo.h`](src/MemoryDemo.h) | Klasy do demonstracji pamięci    |
| [`src/main.cpp`](src/main.cpp)        | Demonstracja stosu i sterty       |
| [`memory_diagram.puml`](memory_diagram.puml) | Diagram UML              |
