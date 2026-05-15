# STL – `std::string` i przetwarzanie tekstu

## Slajd 1: `std::string` vs C-string i SSO

`std::string` to klasa zarządzająca dynamicznym buforem znaków.

```cpp
// C-string – surowy wskaźnik, ręczne zarządzanie
const char* cstr = "Hello";
char buf[6] = "Hello";  // rozmiar musi być znany!

// std::string – zarządza pamięcią automatycznie
std::string s = "Hello";
s += " World";    // automatyczna realokacja
s.size();         // 11 – bez terminalnego '\0'
s.length();       // synonim size()
s.c_str();        // const char* – kompatybilność z C API
```

**SSO (Small String Optimization):**
Większość implementacji przechowuje krótkie stringi (≤ 15–22 znaków)
bezpośrednio w obiekcie — **bez alokacji na stercie**.

```
std::string s = "Hi";        // SSO: dane w buforze inline obiektu
std::string s2 = "To jest już dłuższy tekst..."; // alokacja na stercie
```

---

## Slajd 2: Kluczowe operacje

```cpp
std::string s = "Ala ma kota";

// Dostęp
s[0];             // 'A' – bez kontroli zakresu
s.at(0);          // 'A' – z kontrolą (rzuca out_of_range)
s.front();        // 'A'
s.back();         // 'a'

// Wyszukiwanie
s.find("ma");             // 4 – pozycja lub std::string::npos
s.find("psa");            // std::string::npos
s.rfind("a");             // 10 – ostatnie wystąpienie
s.find_first_of("aeiou"); // 0 – pierwsza samogłoska

// Wycinanie i modyfikacja
s.substr(4, 2);           // "ma" – od pozycji 4, długość 2
s.replace(4, 2, "nie ma"); // "Ala nie ma kota"
s.erase(4, 7);             // usuń 7 znaków od pos 4
s.insert(4, "nie ");       // wstaw w pozycji 4

// Porównanie
s == "Ala ma kota";        // true
s.compare("Ala");          // > 0 (leksykograficznie)
```

---

## Slajd 3: Konwersje liczbowe

```cpp
// string → liczba
std::string s = "42";
int i     = std::stoi(s);      // 42
long l    = std::stol("123");
double d  = std::stod("3.14");
float f   = std::stof("1.5");

// Obsługa błędów
try {
    int x = std::stoi("abc");  // rzuca std::invalid_argument
} catch (const std::invalid_argument& e) {
    std::cout << "Błąd: " << e.what() << "\n";
}
try {
    int x = std::stoi("99999999999");  // rzuca std::out_of_range
} catch (const std::out_of_range& e) {
    std::cout << "Zakres: " << e.what() << "\n";
}

// liczba → string
std::string s2 = std::to_string(42);     // "42"
std::string s3 = std::to_string(3.14);   // "3.140000"
```

---

## Slajd 4: `std::string_view` (C++17) — widok bez kopii

`string_view` to lekki, **nieposiadający zasobów** widok na ciąg znaków.
Nie kopiuje — przechowuje tylko wskaźnik i długość.

```cpp
#include <string_view>

void wyswietl(std::string_view sv) {
    std::cout << sv << " (dł=" << sv.size() << ")\n";
    // sv.data() – wskaźnik na oryginał, BRAK '\0' gwarantowanego!
}

std::string s = "Hello World";
wyswietl(s);              // zero kopii
wyswietl("Hello World");  // działa z C-stringiem
wyswietl({s.data(), 5});  // tylko "Hello"

// Wszystkie operacje odczytu std::string dostępne w string_view:
std::string_view sv = "Ala ma kota";
sv.substr(4, 2);    // string_view na "ma" – zero kopii!
sv.find("ma");      // 4

// UWAGA: string_view nie zarządza życiem danych!
std::string_view niebezpieczny() {
    std::string s = "tymczasowy";
    return s;    // UB! s zostaje zniszczony, string_view zwisa
}
```

---

## Slajd 5: `std::stringstream` — budowanie i parsowanie

```cpp
#include <sstream>

// Budowanie stringa
std::ostringstream oss;
oss << "Wynik: " << 42 << " temp: " << 36.6 << "°C";
std::string wynik = oss.str();
std::cout << wynik << "\n";

// Parsowanie stringa
std::string dane = "Jan 30 95.5";
std::istringstream iss(dane);
std::string imie;
int wiek;
double waga;
iss >> imie >> wiek >> waga;
std::cout << imie << " ma " << wiek << " lat i " << waga << " kg\n";

// Tokenizacja po spacji
std::string zdanie = "to jest zdanie ze slowami";
std::istringstream tokeny(zdanie);
std::string token;
while (tokeny >> token)
    std::cout << "[" << token << "] ";
std::cout << "\n";
```

---

## Slajd 6: C++20 — nowe metody `string`

```cpp
std::string s = "Hello World";

// starts_with / ends_with (C++20)
s.starts_with("Hello");   // true
s.ends_with("World");     // true
s.starts_with("World");   // false

// contains (C++23, ale w wielu implementacjach od C++20)
// Wcześniej:  s.find("llo") != std::string::npos
// Od C++23:   s.contains("llo")  // true

// string_view ma starts_with/ends_with już od C++20:
std::string_view sv = "Hello World";
sv.starts_with("Hello");  // true

// Formatowanie (C++20) – <format>
#include <format>
std::string msg = std::format("Imię: {}, Wiek: {}", "Anna", 30);
std::cout << msg << "\n";  // Imię: Anna, Wiek: 30
```

---

## Pliki źródłowe

| Plik | Opis |
|------|------|
| [`src/main.cpp`](src/main.cpp) | Demonstracja `string`, `string_view`, `stringstream` |
| [`string_diagram.puml`](string_diagram.puml) | Schemat SSO i hierarchia typów string |
| [`string_diagram.png`](string_diagram.png) | Wygenerowany diagram PNG |

![SSO i hierarchia typów string](string_diagram.png)
