#include <iostream>
#include <vector>
#include <algorithm>

// ============================================================
// Demonstracja: Różne kategorie operatorów w C++
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o ops main.cpp && ./ops
// ============================================================

// Macierz 2x2: ilustruje wiele operator naraz
struct Matrix2x2 {
    double a, b, c, d;  // [a b; c d]

    Matrix2x2(double a = 0, double b = 0, double c = 0, double d = 0)
        : a(a), b(b), c(c), d(d) {}

    // Dodawanie macierzy (element po elemencie)
    Matrix2x2 operator+(const Matrix2x2& m) const {
        return {a+m.a, b+m.b, c+m.c, d+m.d};
    }

    // Mnożenie macierzy (iloczyn wierszy przez kolumny)
    Matrix2x2 operator*(const Matrix2x2& m) const {
        return { a*m.a + b*m.c,  a*m.b + b*m.d,
                 c*m.a + d*m.c,  c*m.b + d*m.d };
    }

    // Jednoargumentowe negacja
    Matrix2x2 operator-() const { return {-a, -b, -c, -d}; }

    // Skalowanie (macierz * skalar)
    Matrix2x2 operator*(double s) const { return {a*s, b*s, c*s, d*s}; }

    // Porównanie
    bool operator==(const Matrix2x2& m) const {
        return a == m.a && b == m.b && c == m.c && d == m.d;
    }
    bool operator!=(const Matrix2x2& m) const { return !(*this == m); }

    // Strumień wyjściowy (wolna funkcja)
    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        return os << "[" << m.a << " " << m.b << " / " << m.c << " " << m.d << "]";
    }
};

// Skalowanie z lewej strony: skalar * macierz
Matrix2x2 operator*(double s, const Matrix2x2& m) { return m * s; }

// Prosta klasa z operator< – potrzebna w std::sort
struct NamedValue {
    std::string name;
    int value;
    bool operator<(const NamedValue& o) const { return value < o.value; }
    friend std::ostream& operator<<(std::ostream& os, const NamedValue& nv) {
        return os << nv.name << "=" << nv.value;
    }
};

int main() {
    std::cout << "=== Macierz 2x2 ===\n";
    Matrix2x2 A{1, 2, 3, 4}, B{5, 6, 7, 8};
    std::cout << "A      = " << A      << "\n";
    std::cout << "B      = " << B      << "\n";
    std::cout << "A + B  = " << (A + B)<< "\n";
    std::cout << "A * B  = " << (A * B)<< "\n";
    std::cout << "-A     = " << (-A)   << "\n";
    std::cout << "A * 2  = " << (A * 2.0) << "\n";
    std::cout << "2 * A  = " << (2.0 * A) << "\n";
    std::cout << std::boolalpha;
    std::cout << "A == A = " << (A == A) << "\n";
    std::cout << "A == B = " << (A == B) << "\n";

    std::cout << "\n=== Sortowanie z operator< ===\n";
    std::vector<NamedValue> v = {{"gamma", 3}, {"alpha", 1}, {"beta", 2}};
    std::cout << "Przed: ";
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
    std::sort(v.begin(), v.end());
    std::cout << "Po:    ";
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
