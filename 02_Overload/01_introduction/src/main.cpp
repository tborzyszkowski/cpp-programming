#include <iostream>
#include <string>

// ============================================================
// Demonstracja: Przeciążanie operatorów — motywacja
// ============================================================
// Kompilacja (g++):
//   g++ -std=c++17 -o intro main.cpp && ./intro
// ============================================================

// --- Bez przeciążenia operatorów ----------------------------
struct VecBezOp {
    double x, y;
    VecBezOp(double x, double y) : x(x), y(y) {}
    VecBezOp add(const VecBezOp& o) const { return VecBezOp(x + o.x, y + o.y); }
    VecBezOp subtract(const VecBezOp& o) const { return VecBezOp(x - o.x, y - o.y); }
    VecBezOp scale(double s) const { return VecBezOp(x * s, y * s); }
    bool equals(const VecBezOp& o) const { return x == o.x && y == o.y; }
    void print(const std::string& name) const {
        std::cout << name << " = (" << x << ", " << y << ")\n";
    }
};

// --- Z przeciążeniem operatorów ----------------------------
struct Vec {
    double x, y;
    Vec(double x, double y) : x(x), y(y) {}

    Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y); }
    Vec operator-(const Vec& o) const { return Vec(x - o.x, y - o.y); }
    Vec operator*(double s)     const { return Vec(x * s,   y * s);    }

    bool operator==(const Vec& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vec& o) const { return !(*this == o); }

    // operator<< musi być wolną funkcją (lewy operand to std::ostream)
    friend std::ostream& operator<<(std::ostream& os, const Vec& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};

int main() {
    std::cout << "=== Bez przeciążenia (metody) ===\n";
    VecBezOp a(1.0, 2.0), b(3.0, 4.0);
    VecBezOp c = a.add(b);
    c.print("a.add(b)");
    if (a.equals(b))
        std::cout << "a i b są równe\n";
    else
        std::cout << "a i b są różne\n";

    std::cout << "\n=== Z przeciążeniem (operatory) ===\n";
    Vec u(1.0, 2.0), v(3.0, 4.0);
    std::cout << "u       = " << u        << "\n";
    std::cout << "v       = " << v        << "\n";
    std::cout << "u + v   = " << (u + v)  << "\n";
    std::cout << "v - u   = " << (v - u)  << "\n";
    std::cout << "u * 2.0 = " << (u * 2.0)<< "\n";
    std::cout << std::boolalpha;
    std::cout << "u == u  = " << (u == u) << "\n";
    std::cout << "u == v  = " << (u == v) << "\n";

    // operator+ jest zwykłą funkcją — można ją wywołać jawnie (rzadko używane)
    std::cout << "\n=== operator+ wywołany jawnie ===\n";
    Vec d = u.operator+(v);
    std::cout << "u.operator+(v) = " << d << "\n";

    return 0;
}
