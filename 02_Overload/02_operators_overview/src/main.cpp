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

// operator[] – indeksowanie (musi być metodą klasy)
class IntArray {
    int  size_;
    int* data_;
public:
    explicit IntArray(int size) : size_(size), data_(new int[size]{}) {}
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;
    ~IntArray() { delete[] data_; }

    int& operator[](int idx)             { return data_[idx]; }   // zapis i odczyt
    const int& operator[](int idx) const { return data_[idx]; }   // tylko odczyt (const)
    int size() const { return size_; }
};

// operator() – funktor (obiekt wywoływalny)
struct Multiplier {
    double factor;
    double operator()(double x) const { return x * factor; }
};

// operator T() – konwersja typu
class Celsius {
    double val_;
public:
    explicit Celsius(double v) : val_(v) {}
    explicit operator double() const { return val_; }
    explicit operator bool()   const { return val_ != 0.0; }
    friend std::ostream& operator<<(std::ostream& os, const Celsius& c) {
        return os << c.val_ << " st.C";
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

    std::cout << "\n=== operator[] – indeksowanie ===\n";
    IntArray arr(5);
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 42;
    std::cout << "arr[0] = " << arr[0] << "\n";   // 10
    std::cout << "arr[2] = " << arr[2] << "\n";   // 42

    std::cout << "\n=== operator() – funktor ===\n";
    Multiplier twice{2.0};
    Multiplier half{0.5};
    std::cout << "twice(3.14)  = " << twice(3.14)  << "\n";   // 6.28
    std::cout << "half(100.0)  = " << half(100.0)  << "\n";   // 50.0

    std::cout << "\n=== operator T() – konwersja ===\n";
    Celsius temp(36.6);
    double d = static_cast<double>(temp);   // explicit wymusza static_cast
    std::cout << "temp         = " << temp << "\n";
    std::cout << "as double    = " << d    << "\n";
    std::cout << "as bool      = " << (static_cast<bool>(temp) ? "true" : "false") << "\n";
    Celsius zero(0.0);
    std::cout << "zero as bool = " << (static_cast<bool>(zero) ? "true" : "false") << "\n";

    return 0;
}
