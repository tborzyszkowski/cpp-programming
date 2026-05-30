# Type Erasure

## Slajd 1: Problem – przechowywanie heterogenicznych typów

Szablony w C++ są świetne, ale generują odrębny kod dla każdego `T`.
Czasem potrzebujemy przechować **wiele różnych typów w jednej strukturze danych** – tu wkracza *type erasure*.

```cpp
// Problem: jak przechować różne "wywoływalne" obiekty w jednym kontenerze?

auto lambda1 = [](int x) { return x * 2; };
auto lambda2 = [factor = 3](int x) { return x * factor; };
struct Funktor { int operator()(int x) const { return x + 10; } };

// Każde z nich ma INNY typ! Nie można wrzucić do jednego vector<???>.

// Chcemy:
std::vector<???>  callbacks;
callbacks.push_back(lambda1);
callbacks.push_back(lambda2);
callbacks.push_back(Funktor{});

// Rozwiązanie: type erasure – ukrycie konkretnego typu za wspólnym interfejsem
std::vector<std::function<int(int)>> callbacks;  // std::function robi type erasure
callbacks.push_back(lambda1);
callbacks.push_back(lambda2);
callbacks.push_back(Funktor{});
```

---

## Slajd 2: Type erasure przez wirtualne funkcje

Klasyczne podejście OOP: ukrywamy typ za wskaźnikiem do abstrakcyjnej klasy bazowej.

```cpp
// Interfejs – "wymazuje" konkretny typ
struct IFigura {
    virtual double pole() const = 0;
    virtual void rysuj() const = 0;
    virtual ~IFigura() = default;
};

// Konkretne typy
struct Kolo : IFigura {
    double r;
    explicit Kolo(double r) : r(r) {}
    double pole()  const override { return 3.14 * r * r; }
    void   rysuj() const override { std::cout << "O(r=" << r << ")"; }
};

struct Kwadrat : IFigura {
    double a;
    explicit Kwadrat(double a) : a(a) {}
    double pole()  const override { return a * a; }
    void   rysuj() const override { std::cout << "[" << a << "x" << a << "]"; }
};

// Kolekcja różnych figur – type erasure przez wskaźnik
std::vector<std::unique_ptr<IFigura>> figury;
figury.push_back(std::make_unique<Kolo>(5.0));
figury.push_back(std::make_unique<Kwadrat>(3.0));
figury.push_back(std::make_unique<Kolo>(2.0));

for (const auto& f : figury)
    std::cout << "pole=" << f->pole() << "\n";
```

**Wady:** vtable overhead, heap allocation, konieczność dziedziczenia.

---

## Slajd 3: `std::function` – type erasure dla callable

`std::function<Sig>` przechowuje **dowolny obiekt wywoływalny** z sygnaturą `Sig`:

```cpp
#include <functional>

// std::function akceptuje lambdę, funktor, wskaźnik do funkcji:
std::function<int(int, int)> operacja;

operacja = [](int a, int b) { return a + b; };
std::cout << operacja(3, 4) << "\n";  // 7

operacja = std::multiplies<int>{};    // funktor STL
std::cout << operacja(3, 4) << "\n";  // 12

int mnoznik = 5;
operacja = [mnoznik](int a, int b) { return a * b * mnoznik; };
std::cout << operacja(3, 4) << "\n";  // 60

// Wektor różnych callable – type erasure!
std::vector<std::function<void(std::string)>> handlers;
handlers.push_back([](const std::string& s) { std::cout << "Log: " << s; });
handlers.push_back([](const std::string& s) { std::cerr << "Err: " << s; });

// Implementacja wewnętrzna std::function:
// - Małe obiekty: Small Buffer Optimization (SBO) – bez alokacji sterty
// - Duże obiekty: alokacja na stercie + wskaźnik do vtable
// Koszt: zazwyczaj jedna pośrednia (indirect) call przez wskaźnik
```

---

## Slajd 4: `std::any` – type erasure dla dowolnego typu

`std::any` może przechować **wartość dowolnego typu** (C++17):

```cpp
#include <any>

std::any x;
x = 42;                          // przechowuje int
std::cout << std::any_cast<int>(x) << "\n";

x = std::string{"hello"};        // teraz przechowuje string
std::cout << std::any_cast<std::string>(x) << "\n";

x = std::vector<int>{1,2,3};    // teraz wektor

// Sprawdzanie typu
if (x.type() == typeid(std::vector<int>)) {
    auto& v = std::any_cast<std::vector<int>&>(x);
    std::cout << "wektor, rozmiar=" << v.size() << "\n";
}

// Zły rzut → std::bad_any_cast
try {
    auto s = std::any_cast<int>(x);  // x to wektor, nie int!
} catch (const std::bad_any_cast& e) {
    std::cout << "Błąd: " << e.what() << "\n";
}

// Zastosowanie: heterogeniczna mapa konfiguracji
std::map<std::string, std::any> config;
config["host"]    = std::string{"localhost"};
config["port"]    = 8080;
config["debug"]   = true;
config["timeout"] = 30.5;
```

---

## Slajd 5: `std::variant` – type erasure zamknięty

`std::variant<Ts...>` przechowuje **dokładnie jeden z typów z listy** (C++17):

```cpp
#include <variant>

using Wynik = std::variant<int, std::string, std::vector<int>>;

Wynik w = 42;
w = std::string{"błąd!"};
w = std::vector<int>{1, 2, 3};

// Dostęp przez std::visit – obsługa każdego przypadku:
std::visit([](const auto& v) {
    std::cout << "Wartość: " << v << "\n";   // błąd dla vector (niema <<)
}, w);

// Visitor z przeciążeniami (overload pattern):
struct Visitor {
    void operator()(int x)                        { std::cout << "int: " << x; }
    void operator()(const std::string& s)         { std::cout << "str: " << s; }
    void operator()(const std::vector<int>& v)    {
        std::cout << "vec[" << v.size() << "]";
    }
};
std::visit(Visitor{}, w);

// std::get + std::holds_alternative:
if (std::holds_alternative<int>(w)) {
    std::cout << "jest int: " << std::get<int>(w) << "\n";
}

// Zalety vs std::any:
// ✓ Brak alokacji sterty (przechowuje wartość inline)
// ✓ Sprawdzanie kompletności case (std::visit)
// ✗ Lista typów musi być znana z góry (zamknięta)
```

---

## Slajd 6: Własna implementacja type erasure (bez wirtualnych funkcji)

Zaawansowana technika: **wartościowa semantyka** bez vtable w klasie bazowej:

```cpp
// Technika: external polymorphism / duck-typing type erasure
class Figura {
    struct Concept {
        virtual double pole() const = 0;
        virtual void rysuj() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
        virtual ~Concept() = default;
    };

    template<typename T>
    struct Model : Concept {
        T obj_;
        explicit Model(T x) : obj_(std::move(x)) {}
        double pole()  const override { return obj_.pole(); }
        void   rysuj() const override { obj_.rysuj(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<T>>(obj_);
        }
    };

    std::unique_ptr<Concept> impl_;

public:
    template<typename T>
    Figura(T x) : impl_(std::make_unique<Model<T>>(std::move(x))) {}

    // Wartościowa semantyka – deep copy
    Figura(const Figura& o) : impl_(o.impl_->clone()) {}
    Figura& operator=(Figura o) { std::swap(impl_, o.impl_); return *this; }

    double pole()  const { return impl_->pole(); }
    void   rysuj() const { impl_->rysuj(); }
};
// Teraz Figura akceptuje DOWOLNY typ z pole() i rysuj() – bez dziedziczenia!
```

---

## Slajd 7: Porównanie technik type erasure

| Technika | Typy | Alokacja | Koszt call | Kopiowanie | Typy z góry? |
|----------|------|----------|-----------|-----------|-------------|
| `virtual` dispatch | Nieograniczone | Heap | Indirect | Nie (ptr) | Nie |
| `std::function` | Callable | SBO/Heap | Indirect | Tak | Nie |
| `std::any` | Dowolny | SBO/Heap | `any_cast` | Tak | Nie |
| `std::variant` | Z listy | **Stack** | `std::visit` | Tak | **Tak** |
| Własne TE | Nieograniczone | Heap/SBO | Indirect | Opcjonalnie | Nie |

---

## Slajd 8: Kiedy używać której techniki?

```
┌─────────────────────────────────────────────────────────────────┐
│                   Wybór techniki type erasure                    │
│                                                                  │
│  Znasz wszystkie typy z góry?                                    │
│         ↓ TAK                    ↓ NIE                           │
│   std::variant             Potrzebujesz wartościowej semantyki?  │
│   (zero overhead)               ↓ TAK            ↓ NIE          │
│                            własne TE         virtual + ptr       │
│                            lub std::any                          │
│                                                                  │
│  Tylko callable?  →  std::function (najprostsza opcja)           │
│  Krytyczna wydajność + call?  →  własne TE z SBO                 │
└─────────────────────────────────────────────────────────────────┘
```

**Zasada praktyczna:**
- `std::variant` → gdy typy znane z góry (błędy obsługi, operacje arytmetyczne)
- `std::function` → gdy przechowujesz callback / handler
- `std::any` → konfiguracja, mapy heterogeniczne
- `virtual` dispatch → klasyczny polimorfizm OOP z hierarchią klas
