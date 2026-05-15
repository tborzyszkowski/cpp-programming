# 03_STL – Ćwiczenia praktyczne

Ćwiczenia integrują wiedzę ze wszystkich rozdziałów modułu STL:
kontenery, iteratory, algorytmy, funktory, lambdy, string oraz narzędzia ogólne.

---

## Ćwiczenie 1: Książka telefoniczna (`PhoneBook`)

**Temat:** Kontenery asocjacyjne, algorytmy, operatory

Zaimplementuj klasę `PhoneBook` przechowującą kontakty (imię → numer telefonu).

### Wymagania

| Element | Opis |
|---------|------|
| Kontener | `unordered_map<string, string>` |
| Dodawanie | `add(name, number)` — nadpisuje jeśli istnieje |
| Usuwanie | `remove(name)` — zwraca `bool` (sukces/porażka) |
| Wyszukiwanie | `find(name)` → `optional<string>` (numer lub brak) |
| Liczenie | `count_by_prefix(prefix)` → `int` używając `count_if` |
| Lista | `list_sorted()` → `vector<pair<string,string>>` — alfabetycznie |
| Wydruk | `operator<<` — każdy kontakt w nowej linii |

### Szkielet

```cpp
class PhoneBook {
    std::unordered_map<std::string, std::string> kontakty_;
public:
    void add(const std::string& name, const std::string& number);
    bool remove(const std::string& name);
    std::optional<std::string> find(const std::string& name) const;
    int count_by_prefix(const std::string& prefix) const;
    std::vector<std::pair<std::string,std::string>> list_sorted() const;
    friend std::ostream& operator<<(std::ostream& os, const PhoneBook& pb);
};
```

### Przykładowe użycie

```cpp
PhoneBook pb;
pb.add("Anna",    "111-222-333");
pb.add("Bartek",  "444-555-666");
pb.add("Agata",   "777-888-999");

auto nr = pb.find("Anna");    // optional zawiera "111-222-333"
auto brak = pb.find("Zofia"); // nullopt

std::cout << "Kontakty na A: " << pb.count_by_prefix("A") << "\n"; // 2

std::cout << pb; // posortowane alfabetycznie

pb.remove("Bartek");
std::cout << pb;
```

---

## Ćwiczenie 2: Zbiór danych (`DataSet<T>`)

**Temat:** Szablony, `vector`, algorytmy, `optional`

Zaimplementuj szablon klasy `DataSet<T>` do analizy statystycznej danych.

### Wymagania

| Metoda | Opis |
|--------|------|
| `add(value)` | Dodaj wartość |
| `min()` / `max()` | Minimum/maksimum — zwróć `optional<T>` (pusty zbiór → nullopt) |
| `mean()` | Średnia → `optional<double>` |
| `median()` | Mediana → `optional<double>` (nie modyfikuj oryginału!) |
| `sorted()` | Zwróć posortowaną kopię `vector<T>` |
| `filter(pred)` | Zwróć `vector<T>` spełniający predykat (lambda) |

### Szkielet

```cpp
template<typename T>
class DataSet {
    std::vector<T> dane_;
public:
    void add(const T& v);
    std::optional<T> min() const;
    std::optional<T> max() const;
    std::optional<double> mean() const;
    std::optional<double> median() const;
    std::vector<T> sorted() const;
    template<typename Pred>
    std::vector<T> filter(Pred pred) const;
};
```

### Przykładowe użycie

```cpp
DataSet<int> ds;
ds.add(5); ds.add(3); ds.add(8); ds.add(1); ds.add(9);

std::cout << "min: "    << *ds.min()    << "\n"; // 1
std::cout << "max: "    << *ds.max()    << "\n"; // 9
std::cout << "mean: "   << *ds.mean()   << "\n"; // 5.2
std::cout << "median: " << *ds.median() << "\n"; // 5

auto parzyste = ds.filter([](int x){ return x % 2 == 0; });
// { 8 }

DataSet<int> pusty;
assert(!pusty.min().has_value()); // nullopt dla pustego
```

---

## Ćwiczenie 3: Parser logów (`LogParser`)

**Temat:** `string_view`, `stringstream`, algorytmy, `map`

Zaimplementuj parser pliku logów i raport statystyczny.

### Format wejściowy

```
2024-01-15 10:23:45 INFO  Serwer uruchomiony
2024-01-15 10:24:01 ERROR Błąd połączenia z bazą
2024-01-15 10:24:10 WARN  Wysoki CPU
2024-01-16 09:00:00 INFO  Backup wykonany
```

### Wymagania

Zaimplementuj funkcję `parse_log(filename)` i klasę `LogReport`:

```cpp
struct LogEntry {
    std::string date;   // "2024-01-15"
    std::string time;   // "10:23:45"
    std::string level;  // "INFO" / "WARN" / "ERROR"
    std::string message;
};

class LogReport {
public:
    void add_entry(const LogEntry& e);

    // Liczba wpisów wg poziomu (INFO/WARN/ERROR)
    std::map<std::string, int> count_by_level() const;

    // Dni z błędami ERROR posortowane
    std::vector<std::string> error_days() const;

    // Komunikaty zawierające podany fragment (case-sensitive)
    std::vector<std::string> search(std::string_view fragment) const;

    friend std::ostream& operator<<(std::ostream& os, const LogReport& r);
};

LogReport parse_log(const std::string& filename);
```

### Wskazówki

- Użyj `std::getline` + `std::istringstream` do parsowania każdej linii
- `string_view` w `search()` — zero-copy porównanie fragmentu
- `count_by_level()` — `std::for_each` lub `range-for` + `map`
- `error_days()` — `std::set` do deduplikacji, potem `vector`

---

## Rozwiązania

Przykładowe rozwiązania: [`solutions/`](solutions/)

| Katalog | Ćwiczenie |
|---------|-----------|
| [`solutions/ex1_phonebook/`](solutions/ex1_phonebook/) | Książka telefoniczna |
| [`solutions/ex2_statistics/`](solutions/ex2_statistics/) | Zbiór danych |
| [`solutions/ex3_logparser/`](solutions/ex3_logparser/) | Parser logów |
