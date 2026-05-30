# Lambda – Historia i Motywacja

## Slajd 1: Pierwsze callable – wskaźniki na funkcje (C, C++98)

Już w C można było przekazywać funkcje przez wskaźniki:

```cpp
#include <cstdlib>   // qsort
#include <cstring>   // strcmp

// Wskaźnik na funkcję: int (*)(const void*, const void*)
int porownaj_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int tab[] = {5, 2, 8, 1, 9};
qsort(tab, 5, sizeof(int), porownaj_int);
```

**Ograniczenia wskaźników na funkcje:**
- Brak stanu — funkcja nie „pamięta" niczego między wywołaniami
- Typ jest sztywny — `int (*)(const void*, const void*)` nie pasuje do C++ templates
- Nie można tworzyć ich „w miejscu" — zawsze wymagają osobnej definicji
- Brak inline'owania przez kompilator (wirtualne wywołanie przez pointer)

---

## Slajd 2: C++98 – klasy funktorowe

Kiedy C++ wprowadził szablony, pojawiła się potrzeba callable z **parametryzowalnym stanem**.
Rozwiązaniem były funktory — klasy przeciążające `operator()`:

```cpp
// C++98 – funktor z parametrem (stan w polu klasy)
struct WiekszyNiz {
    int prog;
    explicit WiekszyNiz(int p) : prog(p) {}
    bool operator()(int x) const { return x > prog; }
};

struct Mnoznik {
    double wspolczynnik;
    explicit Mnoznik(double w) : wspolczynnik(w) {}
    double operator()(double x) const { return x * wspolczynnik; }
};

// Użycie:
std::vector<int> v = {1, 3, 5, 7, 9, 11};
int n = std::count_if(v.begin(), v.end(), WiekszyNiz{5});
```

Funktory były **dobrim rozwiązaniem** — kompilator może je łatwo inline'ować,
a stan jest explicite zarządzany. Ale miały poważną wadę:

```cpp
// Problem: definicja DALEKO od miejsca użycia
// W pliku my_predicates.h (20 linii wyżej lub w innym pliku):
struct ZlozonePorownanie {
    std::string prefix;
    int min_dlugosc;
    bool case_sensitive;
    ZlozonePorownanie(std::string p, int m, bool cs)
        : prefix(p), min_dlugosc(m), case_sensitive(cs) {}
    bool operator()(const std::string& s) const {
        if ((int)s.size() < min_dlugosc) return false;
        if (case_sensitive) return s.starts_with(prefix);
        // ... itd.
    }
};

// W pliku main.cpp (100 linii niżej):
std::remove_if(v.begin(), v.end(), ZlozonePorownanie{"abc", 3, true});
// ↑ Czytelnik musi skoczyć do definicji, żeby zrozumieć co to robi
```

---

## Slajd 3: C++03 – `std::bind` i `std::mem_fun` (obejścia)

Komitet standaryzacyjny widział problem. Odpowiedź w C++03 to pomocniki z `<functional>`:

```cpp
#include <functional>

// std::bind1st / std::bind2nd (C++03, usunięte w C++17)
// Wiązanie jednego argumentu funktora
std::vector<int> v = {1, 3, 5, 7, 9};
int n = std::count_if(v.begin(), v.end(),
            std::bind1st(std::less<int>(), 5));
// std::less<int>()(5, x)  →  5 < x  →  x > 5

// std::mem_fun – wywoływanie metody przez wskaźnik
std::vector<std::string*> strs = {new std::string("abc"), new std::string("de")};
std::for_each(strs.begin(), strs.end(), std::mem_fun(&std::string::clear));
```

Składnia `bind1st`/`bind2nd` była **nieczytelna i ograniczona** (tylko 1 lub 2 argumenty).
Biblioteka Boost.Bind rozwiązała problem bardziej elastycznie:

```cpp
// Boost.Bind (prekursor C++11 std::bind)
#include <boost/bind.hpp>

bool wiekszy_niz(int x, int prog) { return x > prog; }

std::count_if(v.begin(), v.end(), boost::bind(wiekszy_niz, _1, 5));
// _1 oznacza "pierwszy argument przekazany przez algorytm"
```

Mimo to każde z tych rozwiązań było **obejściem**, a nie eleganckim rozwiązaniem.

---

## Slajd 4: Geneza lambd – lambda calculus i języki funkcyjne

Termin „lambda" pochodzi z **rachunku lambda** (λ-calculus) opracowanego przez
Alonzo Churcha w latach 30. XX wieku — formalnego systemu opisującego obliczenia
za pomocą funkcji anonimowych.

```
λx. x + 1          ← anonimowa funkcja: weź x, zwróć x+1
(λx. x + 1)(5)     ← aplikacja: podstaw 5 za x → 6
```

Języki funkcyjne zaczęły implementować lambdy dziesiątki lat przed C++:

| Język | Rok lambdy | Składnia |
|-------|-----------|---------|
| **Lisp** | 1958 | `(lambda (x) (+ x 1))` |
| **ML** | 1973 | `fn x => x + 1` |
| **Haskell** | 1990 | `\x -> x + 1` |
| **Python** | 1994 | `lambda x: x + 1` |
| **Java** | 2014 | `x -> x + 1` |
| **C++** | **2011** | `[](int x){ return x + 1; }` |

C++ był **jednym z ostatnich** głównych języków, który otrzymał lambdy — ze względu na
złożoność systemu typów i model pamięci.

---

## Slajd 5: Propozycja do standardu – droga do C++11

Praca nad lambdami w C++ trwała kilka lat:

- **2006** – Gary Powell i Peter Dimov składają propozycję N1958
- **2007** – Doug Gregor i Jaakko Järvi rozwijają propozycję N2329
- **2008** – Komitet akceptuje propozycję N2550 (Jaakko Järvi, John Freeman, Lawrence Crowl)
- **2011** – Lambdy wchodzą do standardu ISO C++11

Kluczowe decyzje projektowe:
1. **Jawne przechwycenie** `[=]`, `[&]` zamiast automatycznego (bezpieczeństwo)
2. **Typ domknięcia jest unikalny i anonimowy** — każda lambda ma własny typ
3. **Zgodność z istniejącymi szablonami** — lambda może zastąpić funktor wszędzie
4. **Wskaźnik na funkcję** — bezstanowa lambda jest konwertowalna na function pointer

```
Propozycja 2006 ──▶ Wersje robocze 2007–2009 ──▶ C++11 (sierpień 2011)
    N1958              N2329 / N2550              ISO/IEC 14882:2011
```

---

## Slajd 6: Ewolucja lambd w standardach C++

| Standard | Rok | Nowości lambd |
|----------|-----|---------------|
| **C++11** | 2011 | Podstawowa składnia, przechwycenie `[=]`/`[&]`/lista, `mutable`, konwersja na function pointer |
| **C++14** | 2014 | Lambdy **generyczne** (`auto` parametry), **inicjalizatory przechwycenia** `[x = expr]` |
| **C++17** | 2017 | `constexpr` lambda, przechwycenie `*this` przez wartość, lambdy jako non-type template params |
| **C++20** | 2020 | Lambdy **szablonowe** `[]<typename T>(){}`, lambdy w unevaluated contexts, `[=,this]` |
| **C++23** | 2023 | **Dedukcja `this`** (`self`), lambdy rekurencyjne bez `std::function` |

Każda rewizja usuwała kolejne ograniczenia i zbliżała lambdy do pełnoprawnych
obiektów wywoływalnych pierwszej klasy.

---

## Slajd 7: Relacja lambda – funktor – porównanie

Lambda jest **cukrem składniowym** dla anonimowego funktora. Kompilator transformuje:

```cpp
int prog = 5;
auto f = [prog](int x) { return x > prog; };
```

w coś równoważnego:

```cpp
// Co generuje kompilator (konceptualnie):
struct __lambda_1 {
    int prog;  // przechwycona kopia
    __lambda_1(int p) : prog(p) {}
    bool operator()(int x) const { return x > prog; }
};

auto f = __lambda_1{prog};
```

**Kluczowa różnica:** typ domknięcia jest **unikalny, anonimowy i niemożliwy do nazwania**.
Można go przechować tylko przez `auto` lub `std::function`.

```cpp
auto f = [](int x){ return x * 2; };      // OK – auto deduuje typ domknięcia
// std::function<int(int)> g = f;          // OK – z kosztem alokacji/wirtualności
// MojTyp h = f;                           // BŁĄD – nie można nazwać typu domknięcia
```
