# Zadania – Przeciążanie operatorów

Poniższe zadania rozwijają zagadnienia omówione w rozdziale **02_Overload**.
Są celowo trudniejsze niż prezentowane przykłady – wymagają samodzielnego
zaprojektowania kompletnych klas z wieloma przeciążonymi operatorami.

> Rozwiązania znajdziesz w katalogu [`solutions/`](solutions/).
> Zajrzyj tam dopiero po samodzielnej próbie!

---

## Zadanie 1 – `Matrix2x2` (operatory arytmetyczne i indeksowania)

### Cel
Zaimplementuj klasę macierzy 2×2 z pełnym zestawem operatorów
arytmetycznych, porównania i indeksowania.

### Co zaimplementować

**Klasa `Matrix2x2`** (plik `Matrix2x2.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `data_` | `double[2][2]` |

Operatory do zaimplementowania:
- `operator+`, `operator-` (macierz ± macierz)
- `operator*` (macierz × macierz, macierz × skalar)
- `operator==`, `operator!=`
- `operator[]` – zwraca wskaźnik na wiersz (`double*`), umożliwiając `m[i][j]`
- `operator<<` (friend) – wypisuje macierz w formacie `[a b; c d]`
- `operator()` – dostęp do elementu `m(i, j)` z kontrolą zakresu

Metody publiczne:
- `Matrix2x2()` – macierz zerowa
- `Matrix2x2(double a, double b, double c, double d)`
- `double determinant() const` – wyznacznik `ad - bc`
- `Matrix2x2 transposed() const` – transpozycja

### Trudniejsze elementy
- `operator[]` zwraca `double*` — pozwala na `m[0][1]`, ale nie sprawdza zakresu
- `operator()` z kontrolą zakresu (rzuca `std::out_of_range`)
- Mnożenie macierzy: wynik `c[i][j] = sum(a[i][k] * b[k][j])`

### Przykładowe wyjście
```
A = [1 2; 3 4]
B = [5 6; 7 8]

A + B = [6 8; 10 12]
A * B = [19 22; 43 50]
A * 2 = [2 4; 6 8]

A[0][1] = 2      (operator[])
A(1, 0) = 3      (operator())

det(A) = -2
A^T    = [1 3; 2 4]

A == A ? true
A == B ? false
```

---

## Zadanie 2 – `Statistics` (funktor z `operator()`)

### Cel
Zaprojektuj klasę-funktor, która gromadzi dane liczbowe (podawane
przez `operator()`) i na żądanie oblicza statystyki.

### Co zaimplementować

**Klasa `Statistics`** (plik `Statistics.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `data_` | `std::vector<double>` |
| `label_` | `std::string` |

Operatory do zaimplementowania:
- `operator()(double val)` – dodaje wartość i **zwraca `*this`** (łańcuchowanie)
- `operator double() const` – **explicit**, zwraca średnią
- `operator bool() const` – **explicit**, `true` jeśli są dane
- `operator<<` (friend) – wypisuje label + statystyki

Metody publiczne:
- `Statistics(const std::string& label)`
- `double mean() const` – średnia arytmetyczna
- `double stddev() const` – odchylenie standardowe (populacyjne)
- `double min() const`, `double max() const`
- `size_t count() const`
- `Statistics operator+(const Statistics&) const` – łączy dwa zbiory danych

### Trudniejsze elementy
- `operator()` zwraca referencję `Statistics&` umożliwiając `s(1.0)(2.0)(3.0)`
- Konwersja `explicit operator double()` wymusza `static_cast<double>(s)`
- Odchylenie standardowe: $\sigma = \sqrt{\frac{1}{N}\sum_{i=1}^{N}(x_i - \bar{x})^2}$
- `operator+` scala dane z dwóch obiektów (np. dwa czujniki → wspólna analiza)

### Przykładowe wyjście
```
=== Łańcuchowe dodawanie ===
Pomiary: [n=5] avg=3.00 stddev=1.41 min=1.00 max=5.00

=== Konwersja ===
Jako double (średnia): 3.00
Jako bool: true
Pusty jako bool: false

=== Scalanie danych ===
Dzień 1 + Dzień 2: [n=8] avg=4.38 stddev=2.07 min=1.00 max=8.00
```

---

## Zadanie 3 – `Temperature` (konwersje i porównania)

### Cel
Zaimplementuj klasę temperatury przechowującą wartość w Kelvinach,
z operatorami konwersji do Celsjusza i Fahrenheita oraz pełnym zestawem
operatorów porównania i arytmetycznych.

### Co zaimplementować

**Klasa `Temperature`** (plik `Temperature.h`):

| Składowa (prywatna) | Typ |
|---|---|
| `kelvin_` | `double` |

Operatory do zaimplementowania:
- `operator+`, `operator-` (różnica temperatur → `double`)
- `operator+=`, `operator-=`
- `operator<`, `operator>`, `operator<=`, `operator>=`, `operator==`, `operator!=`
- `explicit operator double() const` – zwraca wartość w Kelvinach
- `operator<<` (friend) – wypisuje np. `293.15 K (20.00°C)`
- `operator""_K`, `operator""_C`, `operator""_F` – literały użytkownika (C++17)

Metody statyczne (fabryki):
- `static Temperature fromCelsius(double c)` – z °C
- `static Temperature fromFahrenheit(double f)` – z °F

Metody publiczne:
- `Temperature(double kelvin)` – explicit
- `double celsius() const` – konwersja K → °C
- `double fahrenheit() const` – konwersja K → °F

### Wzory konwersji
- $K = °C + 273.15$
- $K = (°F - 32) \times \frac{5}{9} + 273.15$

### Trudniejsze elementy
- Literały użytkownika (`operator""_K`, `operator""_C`, `operator""_F`)
- Walidacja: temperatura nie może być poniżej 0 K (rzucaj `std::invalid_argument`)
- Różnica temperatur zwraca `double` (w Kelvinach), a nie `Temperature`
- Porównania z tolerancją `1e-9` (problemy zmiennoprzecinkowe)

### Przykładowe wyjście
```
=== Tworzenie ===
t1 = 373.15 K (100.00°C)
t2 = 273.15 K (0.00°C)
t3 = 310.93 K (37.78°C)    // 100°F

=== Literały użytkownika ===
100.0_C = 373.15 K (100.00°C)
212.0_F = 373.15 K (100.00°C)
300.0_K = 300.00 K (26.85°C)

=== Porównania ===
100°C > 0°C   ? true
100°C == 212°F? true

=== Arytmetyka ===
t1 - t2 = 100.00 (różnica w K)
t2 + 10 = 283.15 K (10.00°C)
```

---

## Wskazówki ogólne

1. Operatory arytmetyczne implementuj parami: `+` i `+=`, `-` i `-=` itd.
   Najlepiej `operator+` wyrażać przez `operator+=`.
2. Operatory konwersji (`operator T()`) oznaczaj jako `explicit`,
   by unikać niejawnych konwersji.
3. Operatory porównania dla wartości zmiennoprzecinkowych powinny
   uwzględniać tolerancję (epsilon).
4. Literały użytkownika definiuj w osobnym `namespace` i używaj `using`.
5. `operator[]` nie pozwala sprawdzić zakresu — preferuj `operator()`
   gdy kontrola jest ważna.
