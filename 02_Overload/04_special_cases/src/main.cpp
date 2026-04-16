#include <iostream>
#include <string>

// ============================================================
// Demonstracja: Przypadki szczególne przeciążania operatorów
// - operatory jednoargumentowe (+, -, !)
// - inkrementacja/dekrementacja prefix i postfix
// - operatory porównania
// - operatory wejścia/wyjścia << i >>
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o special main.cpp && ./special
// ============================================================

// ─── 1. Operatory jednoargumentowe i inkrementacja ──────────

class Counter {
    int val_;
public:
    explicit Counter(int v = 0) : val_(v) {}
    int value() const { return val_; }

    // Jednoargumentowy +
    Counter operator+() const { return Counter( val_); }
    // Jednoargumentowy – negacja wartości
    Counter operator-() const { return Counter(-val_); }

    // Prefix ++c : inkrementuj i zwróć *this przez referencję
    Counter& operator++() {
        ++val_;
        return *this;
    }
    // Postfix c++ : zapamiętaj stary stan, inkrementuj, zwróć kopię
    Counter operator++(int) {
        Counter old = *this;
        ++val_;
        return old;
    }
    // Prefix --c
    Counter& operator--() {
        --val_;
        return *this;
    }
    // Postfix c--
    Counter operator--(int) {
        Counter old = *this;
        --val_;
        return old;
    }

    friend std::ostream& operator<<(std::ostream& os, const Counter& c) {
        return os << "Counter(" << c.val_ << ")";
    }
};

// ─── 2. Operatory porównania ─────────────────────────────────

class Temperature {
    double celsius_;
public:
    explicit Temperature(double c) : celsius_(c) {}
    double value() const { return celsius_; }

    // Jednoargumentowy: negacja (odzwierciedlenie od 0)
    Temperature operator-() const { return Temperature(-celsius_); }
    // Operator ! – czy temperatura wynosi dokładnie 0?
    bool operator!() const { return celsius_ == 0.0; }

    // Definiuj == i <, resztę implementuj przez nie (zasada DRY)
    bool operator==(const Temperature& o) const { return celsius_ == o.celsius_; }
    bool operator!=(const Temperature& o) const { return !(*this == o); }

    bool operator< (const Temperature& o) const { return celsius_ < o.celsius_; }
    bool operator> (const Temperature& o) const { return o < *this; }       // odwróć
    bool operator<=(const Temperature& o) const { return !(o < *this); }    // negacja odwróconego
    bool operator>=(const Temperature& o) const { return !(*this < o); }    // negacja mniejszości

    friend std::ostream& operator<<(std::ostream& os, const Temperature& t) {
        return os << t.celsius_ << "\xB0" "C";  // °C
    }
};

// ─── 3. Operatory wejścia/wyjścia ────────────────────────────

struct Point2D {
    int x, y;
    Point2D(int x = 0, int y = 0) : x(x), y(y) {}

    // operator<< musi być wolną funkcją (lewa strona = ostream, nie Point2D)
    friend std::ostream& operator<<(std::ostream& os, const Point2D& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
    // operator>> musi być wolną funkcją, parametr NIE-const (musimy zapisać dane)
    friend std::istream& operator>>(std::istream& is, Point2D& p) {
        return is >> p.x >> p.y;
    }
};

int main() {
    std::cout << "=== Inkrementacja i dekrementacja ===\n";
    Counter c(5);
    std::cout << "c         = " << c   << "\n";
    std::cout << "++c       = " << ++c  << "  (c teraz: " << c << ")\n";
    std::cout << "c++       = " << c++  << "  (c teraz: " << c << ")\n";
    std::cout << "--c       = " << --c  << "  (c teraz: " << c << ")\n";
    std::cout << "c--       = " << c--  << "  (c teraz: " << c << ")\n";
    std::cout << "+c        = " << +c  << "\n";
    std::cout << "-c        = " << -c  << "\n";

    std::cout << "\n=== Operatory porównania ===\n";
    Temperature t1(20.0), t2(37.0), t3(20.0);
    std::cout << std::boolalpha;
    std::cout << "t1=" << t1 << "  t2=" << t2 << "  t3=" << t3 << "\n";
    std::cout << "t1 == t3: " << (t1 == t3) << "\n";
    std::cout << "t1 != t2: " << (t1 != t2) << "\n";
    std::cout << "t1 <  t2: " << (t1 <  t2) << "\n";
    std::cout << "t2 >  t1: " << (t2 >  t1) << "\n";
    std::cout << "t1 <= t3: " << (t1 <= t3) << "\n";
    std::cout << "t1 >= t3: " << (t1 >= t3) << "\n";
    std::cout << "!t1:      " << (!t1)       << "\n";  // t1 != 0
    std::cout << "-t1:      " << (-t1)       << "\n";  // -20°C

    std::cout << "\n=== Operatory wejścia/wyjścia ===\n";
    Point2D p(3, 7);
    std::cout << "p = " << p << "\n";

    // Łańcuchowanie: każde wywołanie operator<< zwraca referencję do cout
    Point2D q(10, 20);
    std::cout << "p i q = " << p << " oraz " << q << "\n";

    std::cout << "Podaj nowy punkt (x y): ";
    std::cin >> p;
    std::cout << "Nowy punkt: " << p << "\n";

    return 0;
}
