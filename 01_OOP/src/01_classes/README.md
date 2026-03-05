# Klasa i Obiekt w C++

## Slajd 1: Czym jest klasa?

**Klasa** to szablon (typ użytkownika) opisujący:
- **dane** – nazywane *polami* lub *atrybutami* (`double x_`)  
- **zachowanie** – nazywane *metodami* (`double distanceFromOrigin()`)

**Obiekt** to konkretna instancja klasy tworzona w czasie wykonania programu.

```
Klasa  →  „przepis"  →  blueprint
Obiekt →  „ciasto"   →  konkrety w pamięci
```

---

## Slajd 2: Anatomia klasy

```cpp
class Point {
private:            // sekcja prywatna – dostęp tylko z wnętrza klasy
    double x_;
    double y_;

public:             // sekcja publiczna – dostęp z zewnątrz
    Point(double x, double y) : x_(x), y_(y) {}  // konstruktor

    double x() const { return x_; }   // getter
    void setX(double x) { x_ = x; }   // setter

    std::string toString() const;      // deklaracja metody
};
```

**Lista inicjalizacyjna** (`: x_(x), y_(y)`) jest preferowanym sposobem inicjalizacji pól – działa przed wejściem do ciała konstruktora.

---

## Slajd 3: Tworzenie obiektów

```cpp
// Na stosie – automatyczne zarządzanie pamięcią
Point p1;              // konstruktor domyślny
Point p2(3.0, 4.0);    // konstruktor parametryczny

// Na stercie – wymagane ręczne zwolnienie pamięci
Point* p3 = new Point(1.0, 2.0);
delete p3;
p3 = nullptr;
```

> **Ważne:** C++ nie ma odśmiecacza pamięci (garbage collector).  
> Każdy `new` musi mieć odpowiadający `delete`.

---

## Slajd 4: Enkapsulacja i modyfikatory dostępu

| Modyfikator   | Dostępność                          |
|---------------|-------------------------------------|
| `private`     | tylko wewnątrz klasy                |
| `protected`   | w klasie i klasach pochodnych       |
| `public`      | wszędzie                            |

Enkapsulacja chroni wewnętrzny stan obiektu: pola są `private`, dostęp tylko przez metody publiczne.

---

## Slajd 5: Słowo kluczowe `this`

`this` to wskaźnik na bieżący obiekt – używany gdy nazwa parametru pokrywa się z polem:

```cpp
void setX(double x) {
    this->x_ = x;   // this->x_ = pole, x = parametr
}
```

---

## Slajd 6: Diagram klas UML

![Diagram klas](class_diagram.puml)

<!-- Wygeneruj PNG z PlantUML: plantuml class_diagram.puml -->

```
Point                        Rectangle
──────────────────           ────────────────────────────
- x_: double                 - topLeft_: Point
- y_: double                 - bottomRight_: Point
──────────────────           - color_: string
+ Point()                    ────────────────────────────
+ Point(x, y)                + Rectangle(tl, br, color)
+ x(): double                + width(): double
+ y(): double                + height(): double
+ setX(x): void              + area(): double
+ distanceFromOrigin()       + perimeter(): double
+ toString(): string         + toString(): string
```

---

## Slajd 7: Pełny przykład – Point

Plik: [`src/Point.h`](src/Point.h)

```cpp
class Point {
private:
    double x_;
    double y_;
public:
    Point() : x_(0.0), y_(0.0) {}
    Point(double x, double y) : x_(x), y_(y) {}
    ~Point() { std::cout << "[Point] Destruktor\n"; }

    double x() const { return x_; }
    double y() const { return y_; }
    void setX(double x) { x_ = x; }
    void setY(double y) { y_ = y; }

    double distanceFromOrigin() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }
    std::string toString() const {
        return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
    }
};
```

---

## Slajd 8: Pełny przykład – Rectangle

Plik: [`src/Rectangle.h`](src/Rectangle.h)

```cpp
class Rectangle {
private:
    Point topLeft_;
    Point bottomRight_;
    std::string color_;
public:
    Rectangle(const Point& topLeft, const Point& bottomRight,
              const std::string& color = "white")
        : topLeft_(topLeft), bottomRight_(bottomRight), color_(color) {}

    double width()  const { return std::abs(bottomRight_.x() - topLeft_.x()); }
    double height() const { return std::abs(bottomRight_.y() - topLeft_.y()); }
    double area()   const { return width() * height(); }
};
```

Obiekt `Rectangle` **posiada** (kompozycja) dwa obiekty `Point` – jest to relacja *has-a*.

---

## Slajd 9: Program główny

Plik: [`src/main.cpp`](src/main.cpp)

```cpp
int main() {
    Point p1(3.0, 4.0);
    std::cout << p1.toString() << "\n";
    std::cout << "Odległość od O: " << p1.distanceFromOrigin() << "\n";

    Rectangle rect(Point(0,5), Point(4,0), "blue");
    std::cout << "Pole: " << rect.area() << "\n";  // 20

    Point* hp = new Point(7.0, 24.0);
    delete hp;       // obowiązkowe!
}
```

---

## Slajd 10: Kompilacja i uruchomienie

```bash
# Windows (MinGW / MSVC / Clang-cl)
g++ -std=c++17 -o classes src/main.cpp
./classes

# CMake (opcjonalnie)
cmake -S . -B build
cmake --build build
./build/classes
```

Oczekiwane wyjście zawiera komunikaty z konstruktorów i destruktorów – dzięki temu widać cykl życia obiektu.

---

## Podsumowanie

| Pojęcie        | Znaczenie                                              |
|----------------|--------------------------------------------------------|
| Klasa          | Typ użytkownika – szablon pól i metod                 |
| Obiekt         | Instancja klasy w pamięci                             |
| Pole           | Dana przechowywana przez obiekt                       |
| Metoda         | Funkcja należąca do klasy, operująca na `this`        |
| Enkapsulacja   | Ukrywanie danych za interfejsem publicznym            |
| Konstruktor    | Specjalna metoda inicjalizująca obiekt                |
| Destruktor     | Specjalna metoda wywoływana przy niszczeniu obiektu   |

---

## Pliki źródłowe

| Plik                       | Opis                                  |
|----------------------------|---------------------------------------|
| [`src/Point.h`](src/Point.h)         | Klasa `Point` – punkt 2D              |
| [`src/Rectangle.h`](src/Rectangle.h) | Klasa `Rectangle` – prostokąt         |
| [`src/main.cpp`](src/main.cpp)       | Program demonstracyjny                |
| [`class_diagram.puml`](class_diagram.puml) | Diagram UML klas                |
