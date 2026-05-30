# 04_Lambda – Ćwiczenia praktyczne

Ćwiczenia integrują wiedzę ze wszystkich rozdziałów modułu Lambda:
historię callables, składnię, przechwycenie, zaawansowane cechy i zastosowania praktyczne.

---

## Ćwiczenie 1: `FunctionPipeline<T>` – łańcuch transformacji

**Temat:** Lambdy jako callbacki, generyczność, `std::function`, kompozycja

Zaimplementuj klasę szablonową `FunctionPipeline<T>` reprezentującą łańcuch transformacji.

### Wymagania

| Element | Opis |
|---------|------|
| Typ | `FunctionPipeline<T>` — działa na wartościach typu `T` |
| `pipe(f)` | Dodaje etap transformacji `T → T`; zwraca `*this` (fluent interface) |
| `pipe_if(pred, f)` | Dodaje etap warunkowy — stosuje `f` tylko gdy `pred(x)` jest true |
| `run(T value)` | Przepuszcza wartość przez cały łańcuch, zwraca wynik |
| `reset()` | Czyści wszystkie etapy |
| Przechwycenie | Etapy mogą przechwytywać zmienne zewnętrzne |

### Szkielet

```cpp
template<typename T>
class FunctionPipeline {
    using Step = std::function<T(T)>;
    std::vector<Step> steps_;
public:
    FunctionPipeline& pipe(Step f);
    FunctionPipeline& pipe_if(std::function<bool(T)> pred, Step f);
    T run(T value) const;
    void reset();
};
```

### Przykładowe użycie

```cpp
FunctionPipeline<int> p;
int mnoznik = 3;

p.pipe([](int x){ return x * 2; })
 .pipe([mnoznik](int x){ return x + mnoznik; })
 .pipe_if([](int x){ return x > 10; }, [](int x){ return x - 5; })
 .pipe([](int x){ return x * x; });

std::cout << p.run(3);   // (3*2+3) = 9, 9 < 10 → skip if, 9*9 = 81
std::cout << p.run(10);  // (10*2+3) = 23, 23 > 10 → 23-5=18, 18*18 = 324

// Pipeline z innymi typami:
FunctionPipeline<std::string> sp;
sp.pipe([](std::string s){ std::transform(s.begin(),s.end(),s.begin(),::toupper); return s; })
  .pipe([](std::string s){ return "[" + s + "]"; });
std::cout << sp.run("hello");  // "[HELLO]"
```

---

## Ćwiczenie 2: `Scheduler` – harmonogram zadań z opóźnieniami

**Temat:** Lambdy jako zadania, przechwycenie, IIFE, `std::function`

Zaimplementuj klasę `Scheduler` zarządzającą zadaniami opóźnionymi (symulowanymi).

### Wymagania

| Element | Opis |
|---------|------|
| `schedule(delay, task)` | Planuje zadanie `task` (lambda `void()`) po `delay` tickach |
| `tick()` | Zmniejsza opóźnienie wszystkich zadań o 1; wykonuje te z delay=0 |
| `pending_count()` | Liczba oczekujących zadań |
| `cancel_all()` | Anuluje wszystkie zaplanowane zadania |
| Kolejność | Zadania z tym samym `delay` wykonywane w kolejności dodania |

### Szkielet

```cpp
class Scheduler {
    struct Task {
        int delay;
        std::function<void()> action;
    };
    std::vector<Task> queue_;
public:
    void schedule(int delay, std::function<void()> task);
    void tick();
    int pending_count() const;
    void cancel_all();
};
```

### Przykładowe użycie

```cpp
Scheduler s;
std::vector<std::string> log;

s.schedule(0, [&log]{ log.push_back("natychmiast"); });
s.schedule(2, [&log]{ log.push_back("po 2 tickach"); });
s.schedule(1, [&log]{ log.push_back("po 1 ticku"); });
s.schedule(2, [&log]{ log.push_back("po 2 tickach (drugie)"); });

s.tick();  // wykonuje: "natychmiast"
           // log: {"natychmiast"}
           // oczekujące: 2 (delay=1, delay=1 dla obu 2-tickowych)

s.tick();  // wykonuje: "po 1 ticku"
           // log: {"natychmiast", "po 1 ticku"}

s.tick();  // wykonuje: "po 2 tickach", "po 2 tickach (drugie)"
           // log: {..., "po 2 tickach", "po 2 tickach (drugie)"}
```

---

## Ćwiczenie 3: `QueryBuilder` – DSL do filtrowania kolekcji

**Temat:** Lambdy jako predykaty, generyczność, przechwycenie, kompozycja

Zaimplementuj `QueryBuilder<T>` — płynne API do filtrowania i transformacji wektorów.

### Wymagania

| Metoda | Opis |
|--------|------|
| `QueryBuilder(collection)` | Konstruktor — kopiuje lub przenosi kolekcję |
| `where(pred)` | Filtruje elementy spełniające predykat |
| `order_by(comp)` | Sortuje według komparatora |
| `transform(f)` | Przekształca każdy element funkcją `T → T` |
| `take(n)` | Bierze pierwsze `n` elementów |
| `skip(n)` | Pomija pierwsze `n` elementów |
| `to_vector()` | Zwraca wynik jako `std::vector<T>` |
| `count()` | Zwraca liczbę elementów po filtrach |
| `first()` | Zwraca `std::optional<T>` — pierwszy pasujący lub `nullopt` |

### Szkielet

```cpp
template<typename T>
class QueryBuilder {
    std::vector<T> data_;
public:
    explicit QueryBuilder(std::vector<T> data);

    QueryBuilder& where(std::function<bool(const T&)> pred);
    QueryBuilder& order_by(std::function<bool(const T&, const T&)> comp);
    QueryBuilder& transform(std::function<T(T)> f);
    QueryBuilder& take(int n);
    QueryBuilder& skip(int n);

    std::vector<T> to_vector() const;
    int count() const;
    std::optional<T> first() const;
};
```

### Przykładowe użycie

```cpp
struct Student { std::string imie; int ocena; std::string kierunek; };

std::vector<Student> studenci = {
    {"Anna",   85, "Informatyka"},
    {"Bartek", 72, "Matematyka"},
    {"Celina", 91, "Informatyka"},
    {"Damian", 65, "Fizyka"},
    {"Ewa",    88, "Informatyka"},
    {"Filip",  79, "Matematyka"},
};

auto wynik = QueryBuilder(studenci)
    .where([](const auto& s){ return s.kierunek == "Informatyka"; })
    .where([](const auto& s){ return s.ocena >= 80; })
    .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
    .to_vector();

// wynik: {{"Celina",91,"Informatyka"}, {"Ewa",88,"Informatyka"}, {"Anna",85,"Informatyka"}}

auto najlepszy = QueryBuilder(studenci)
    .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
    .first();  // optional<Student>{"Celina", 91, ...}

int ile_info = QueryBuilder(studenci)
    .where([](const auto& s){ return s.kierunek == "Informatyka"; })
    .count();  // 3
```
