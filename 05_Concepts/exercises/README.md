# Zadania – Szablony, Concepts i Type Erasure

## Zadanie 1: `Stack<T>` z Concepts

Zaimplementuj generyczny stos z ograniczeniami typów przez Concepts.

### Wymagania

Zdefiniuj concept `Stosowalny` (ang. *Stackable*) i klasę szablonową `Stack<T, N>`:

```cpp
// Concept: T musi być copyable i equality_comparable
template<typename T>
concept Stosowalny = std::copyable<T> && std::equality_comparable<T>;

// Stack z limitem rozmiaru
template<Stosowalny T, std::size_t N = 64>
class Stack {
public:
    void        push(const T& x);       // wrzuć element (throw gdy pełny)
    void        push(T&& x);            // wrzuć przez move
    T           pop();                  // zdejmij i zwróć (throw gdy pusty)
    const T&    top() const;            // podejrzyj wierzchołek (throw gdy pusty)
    bool        empty()    const;
    bool        full()     const;
    std::size_t size()     const;
    std::size_t capacity() const;

    // Dodatkowe:
    bool        contains(const T& x) const;  // wymaga equality_comparable
    void        clear();
};
```

### Wymagania techniczne

1. Implementacja **bez dynamicznej alokacji** (`std::array<T, N>` wewnętrznie)
2. Silne gwarancje wyjątków dla `push`/`pop` (no-leak)
3. Zdefiniuj własny wyjątek `StackOverflow` i `StackUnderflow`
4. `contains()` musi działać dzięki `equality_comparable` z conceptu

### Przykład użycia

```cpp
Stack<int, 4> s;
s.push(1); s.push(2); s.push(3);
std::cout << s.top();       // 3
std::cout << s.contains(2); // true
s.push(4);
// s.push(5);               // StackOverflow!

// Concept odrzuca niekopiowalne typy:
// Stack<std::unique_ptr<int>> ups;  // BŁĄD kompilacji!
```

---

## Zadanie 2: `TypeSafeRegistry` z `std::variant`

Zaimplementuj rejestr heterogenicznych wartości używając `std::variant`.

### Wymagania

```cpp
// Zarejestrowane typy (znane z góry):
using Wartosc = std::variant<int, double, std::string, bool>;

class TypeSafeRegistry {
public:
    void   ustaw(const std::string& klucz, Wartosc wartosc);
    Wartosc pobierz(const std::string& klucz) const;   // throw gdy brak klucza

    // Typowane pobieranie z concept:
    template<typename T>
        requires (std::same_as<T, int>    ||
                  std::same_as<T, double> ||
                  std::same_as<T, std::string> ||
                  std::same_as<T, bool>)
    T pobierz_jako(const std::string& klucz) const;   // throw gdy zły typ

    bool  zawiera(const std::string& klucz) const;
    bool  jest_typu(const std::string& klucz, std::type_index typ) const;
    void  usun(const std::string& klucz);
    void  dla_kazdego(const std::function<void(const std::string&, const Wartosc&)>& f) const;

    // Statystyki
    std::size_t  rozmiar() const;
    std::map<std::string, std::string> typy() const;  // klucz → nazwa typu
};
```

### Wymagania techniczne

1. Użyj `std::unordered_map<std::string, Wartosc>` jako storage
2. Metoda `typy()` zwraca mapę: klucz → `"int"`, `"double"`, `"string"`, `"bool"`
3. `dla_kazdego` wywołuje funktor w kolejności alfabetycznej kluczy
4. Zdefiniuj wyjątek `RegistryKeyError` i `RegistryTypeError`

### Przykład użycia

```cpp
TypeSafeRegistry reg;
reg.ustaw("host",    std::string{"localhost"});
reg.ustaw("port",    8080);
reg.ustaw("debug",   true);
reg.ustaw("timeout", 30.5);

std::cout << reg.pobierz_jako<std::string>("host");   // "localhost"
std::cout << reg.pobierz_jako<int>("port");            // 8080

// Zły typ:
// reg.pobierz_jako<double>("port");  // RegistryTypeError!

reg.dla_kazdego([](const std::string& k, const auto& v) {
    std::cout << k << ": ";
    std::visit([](const auto& x) { std::cout << x; }, v);
    std::cout << "\n";
});
```

---

## Zadanie 3: Generyczny Logger z Concepts

Zaimplementuj system logowania z concepts definiującymi wymagania typów.

### Wymagania

```cpp
// Concept dla "drukowalnego" obiektu
template<typename T>
concept Drukowalny = requires(T x, std::ostream& os) {
    { os << x } -> std::same_as<std::ostream&>;
};

// Concept dla "serializowalnego" obiektu
template<typename T>
concept Serializowalny = requires(T x) {
    { x.serialize() } -> std::convertible_to<std::string>;
};

// Poziomy logowania
enum class Poziom { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    // Logowanie Drukowalnych (przez <<)
    template<Drukowalny T>
    void log(Poziom poziom, const T& wiadomosc);

    // Logowanie Serializowalnych (przez serialize())
    template<Serializowalny T>
    void log_obj(Poziom poziom, const T& obiekt);

    // Logowanie z kontekstem (variadic template)
    template<Drukowalny... Ts>
    void log_kontekst(Poziom poziom, Ts&&... czesci);

    // Konfiguracja
    void ustaw_min_poziom(Poziom p);
    void dodaj_wyjscie(std::function<void(const std::string&)> wyjscie);

    // Statystyki
    std::map<Poziom, int> statystyki() const;
};
```

### Wymagania techniczne

1. Logger może mieć wiele wyjść (wiele sink'ów) — wektor `std::function<void(const std::string&)>`
2. Każdy log jest formatowany jako: `[POZIOM] [timestamp] wiadomość`
3. Minimalne timestamp to numer sekwencyjny (nie musisz używać `std::chrono`)
4. `log_kontekst` łączy wszystkie `Ts` w jeden string przez `std::ostringstream`
5. Testy: sprawdź, że brak `operator<<` powoduje błąd kompilacji (zakomentowane)

### Przykład użycia

```cpp
Logger logger;
logger.ustaw_min_poziom(Poziom::INFO);

// Dodanie sink'ów:
logger.dodaj_wyjscie([](const std::string& s) { std::cout << s << "\n"; });
logger.dodaj_wyjscie([](const std::string& s) { /* zapis do pliku... */ });

logger.log(Poziom::INFO,  std::string{"Aplikacja startuje"});
logger.log(Poziom::DEBUG, 42);       // ignorowane – poniżej min_poziom
logger.log(Poziom::WARN,  3.14);     // double jest Drukowalny

// Obiekt z serialize():
struct Zdarzenie {
    std::string typ; int id;
    std::string serialize() const {
        return "Zdarzenie{typ=" + typ + ",id=" + std::to_string(id) + "}";
    }
};
logger.log_obj(Poziom::ERROR, Zdarzenie{"klik", 42});

// Variadic:
logger.log_kontekst(Poziom::INFO, "User ", 99, " zalogował się z ", "127.0.0.1");

auto stats = logger.statystyki();
std::cout << "INFO: " << stats[Poziom::INFO] << " logi\n";
```
