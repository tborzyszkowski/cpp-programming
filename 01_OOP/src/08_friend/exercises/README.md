# Zadania – Funkcje i Klasy Zaprzyjaźnione (`friend`)

Poniższe zadania rozwijają zagadnienia omówione w lekcji.
Są celowo trudniejsze niż prezentowane przykłady – wymagają samodzielnego
zaprojektowania relacji `friend` między klasami.

> Rozwiązania znajdziesz w katalogu [`solutions/`](solutions/).
> Zajrzyj tam dopiero po samodzielnej próbie!

---

## Zadanie 1 – `BankAccount` i `Auditor`

### Cel
Zamodeluj konto bankowe oraz klasę audytora, który może czytać
**prywatną historię transakcji** bez ingerowania w publiczny interfejs konta.

### Co zaimplementować

**Klasa `BankAccount`** (plik `BankAccount.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `owner_` | `std::string` |
| `balance_` | `double` |
| `transactions_` | `std::vector<double>` |

Metody publiczne:
- `BankAccount(owner, initialBalance)` – konstruktor
- `void deposit(double amount)` – wpłata (zapisuje transakcję)
- `bool withdraw(double amount)` – wypłata (zwraca `false` gdy brak środków)
- `friend std::ostream& operator<<` – wypisuje właściciela i saldo

Zaprzyjaźnienia:
- `friend class Auditor`
- `friend std::ostream& operator<<(std::ostream&, const BankAccount&)`

**Klasa `Auditor`** (w tym samym pliku):

| Składowa (prywatna) | Typ |
|---|---|
| `auditorName_` | `std::string` |

Metody publiczne:
- `void generateReport(const BankAccount&) const` – wypisuje pełen raport
  (właściciel, saldo, wszystkie transakcje, min/max/avg)
- `double averageTransaction(const BankAccount&) const`
- `bool hasOverdraftAttempt(const BankAccount&) const`
  *(podpowiedź: transakcje z wartością ujemną, które* **nie** *zmieniły salda – musisz to zaprojektować)*

### Trudniejsze elementy względem przykładu
- `Auditor` musi czytać `std::vector<double>` prywatnych transakcji
- `operator<<` jako `friend` globalnej funkcji
- Logika `withdraw` – zapisywanie **próby** wypłaty też jako transakcję (ze znakiem ujemnym gdy nieudana; **ze znakiem ujemnym i flagą** gdy udana)

### Przykładowe wyjście
```
=== Raport Audytora: Jan Kowalski ===
Saldo końcowe : 850.00 PLN
Liczba transakcji: 4
  +500.00  +200.00  -300.00  +450.00
Min: -300.00  Max: +500.00  Avg: +212.50
Próby przekroczenia salda: 0
```

---

## Zadanie 2 – `Vector2D` i `Matrix2x2`

### Cel
Zaimplementuj klasy wektora i macierzy 2×2 tak, by `operator*`
(mnożenie macierzy przez wektor) był **zaprzyjaźniony z obiema klasami jednocześnie**.
To klasyczny wzorzec gdzie jedna zewnętrzna funkcja potrzebuje dostępu do
prywatnych składowych **dwóch różnych klas**.

### Co zaimplementować

**Klasa `Vector2D`** (plik `Vector2D.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `x_` | `double` |
| `y_` | `double` |

Metody publiczne:
- `Vector2D(double x, double y)`
- `double length() const` – długość euklidesowa
- `Vector2D operator+(const Vector2D&) const`
- `friend std::ostream& operator<<`
- `friend Vector2D operator*(const Matrix2x2&, const Vector2D&)` ← kluczowe

**Klasa `Matrix2x2`** (plik `Matrix2x2.h`):

Przechowuje macierz 2×2: `a b / c d` jako prywatne pola `a_, b_, c_, d_`.

Metody publiczne:
- `Matrix2x2(double a, double b, double c, double d)`
- `Matrix2x2 operator*(const Matrix2x2&) const` – mnożenie macierzy (opcjonalne)
- `friend std::ostream& operator<<`
- `friend Vector2D operator*(const Matrix2x2&, const Vector2D&)` ← kluczowe

**Wolna funkcja zaprzyjaźniona z obiema klasami:**
```cpp
Vector2D operator*(const Matrix2x2& m, const Vector2D& v);
// m.a_ * v.x_ + m.b_ * v.y_
// m.c_ * v.x_ + m.d_ * v.y_
```

### Trudniejsze elementy względem przykładu
- `operator*` musi być zadeklarowany jako `friend` w **obu** klasach
- Wymaga **forward declaration** (wzajemne odwołanie klas)
- Mnożenie macierzy jako metoda (opcjonalne rozszerzenie)

### Przykładowe wyjście
```
v = (3.00, 4.00)   |v| = 5.00
m = [2  0; 0  3]

m * v = (6.00, 12.00)

Rotacja 90°: (-4.00, 3.00)
```

---

## Zadanie 3 – `TemperatureSensor` i `DataExporter`

### Cel
Zamodeluj czujnik temperatury z **prywatną historią odczytów** oraz
klasę eksportera danych, która może serializować surowe dane wewnętrzne.
Eksporter NIE powinien mieć dostępu przez publiczne gettery (nie ma ich
dla surowej tablicy odczytów).

### Co zaimplementować

**Klasa `TemperatureSensor`** (plik `TemperatureSensor.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `sensorId_` | `std::string` |
| `readings_` | `std::vector<double>` |
| `threshold_` | `double` (próg alarmu) |

Metody publiczne:
- `TemperatureSensor(id, threshold)`
- `void addReading(double temp)` – dodaje odczyt; jeśli > threshold wypisuje ostrzeżenie
- `double current() const` – ostatni odczyt (lub `NaN` gdy brak)
- `friend class DataExporter`

**Klasa `DataExporter`**:

Metody publiczne:
- `void exportCSV(const TemperatureSensor&, const std::string& filename) const`
  – zapisuje do pliku CSV: `timestamp_idx,temperature`
- `void printStats(const TemperatureSensor&) const`
  – wypisuje: min, max, średnia, liczba przekroczeń progu
- `std::vector<double> anomalies(const TemperatureSensor&) const`
  – zwraca odczyty powyżej `threshold_` (dostęp do prywatnego pola!)

### Trudniejsze elementy względem przykładu
- `DataExporter` czyta prywatny `threshold_` (nie tylko dane, ale i parametr konfiguracyjny)
- Zapis do pliku (`std::ofstream`)
- Operacje statystyczne na kolekcji prywatnych danych

### Przykładowe wyjście
```
[ALARM] Sensor A1: 87.50°C przekracza próg 80.00°C!
[ALARM] Sensor A1: 92.10°C przekracza próg 80.00°C!

=== Statystyki czujnika A1 ===
Liczba odczytów : 6
Min / Max       : 21.30 / 92.10 °C
Średnia         : 54.47 °C
Przekroczenia   : 2

Anomalie: 87.50  92.10
Eksport → sensor_A1.csv (6 rekordów)
```

---

## Wskazówki ogólne

1. Zawsze **deklaruj zaprzyjaźnienie wewnątrz klasy**, której prywatne dane udostępniasz – nie na odwrót.
2. Przy wzajemnym odwołaniu dwóch klas (`Vector2D` ↔ `Matrix2x2`) użyj **forward declaration** przed definicją pierwszej klasy.
3. `friend` nie jest dziedziczone – zaprzyjaźnienie obowiązuje tylko dla wskazanej klasy, nie jej dzieci.
4. Przed użyciem `friend` sprawdź: *„czy mógłbym rozwiązać to publicznym getterem?"*. Jeśli tak – nie używaj `friend`.
