#pragma once
#include <iostream>
#include <iomanip>
#include "Vector2D.h"

// =============================================================
// Klasa Matrix2x2  – macierz 2×2:  [ a  b ]
//                                   [ c  d ]
// =============================================================
class Matrix2x2 {
private:
    double a_, b_;   // wiersz 1
    double c_, d_;   // wiersz 2

public:
    Matrix2x2(double a, double b, double c, double d)
        : a_(a), b_(b), c_(c), d_(d) {}

    // Mnożenie macierzy przez macierz
    Matrix2x2 operator*(const Matrix2x2& o) const {
        return { a_*o.a_ + b_*o.c_,  a_*o.b_ + b_*o.d_,
                 c_*o.a_ + d_*o.c_,  c_*o.b_ + d_*o.d_ };
    }

    double determinant() const { return a_ * d_ - b_ * c_; }

    // ---- zaprzyjaźnienia ----
    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m);

    // Ten sam operator jest friend zarówno w Vector2D jak i Matrix2x2
    friend Vector2D operator*(const Matrix2x2& m, const Vector2D& v);
};

inline std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
    os << std::fixed << std::setprecision(2)
       << "[ " << m.a_ << "  " << m.b_ << " ]\n"
       << "[ " << m.c_ << "  " << m.d_ << " ]";
    return os;
}

// =============================================================
// Wolna funkcja zaprzyjaźniona z OBIEMA klasami jednocześnie.
// Dostęp do prywatnych pól Vector2D (x_, y_) i Matrix2x2 (a_ b_ c_ d_)
// =============================================================
inline Vector2D operator*(const Matrix2x2& m, const Vector2D& v) {
    return { m.a_ * v.x_ + m.b_ * v.y_,
             m.c_ * v.x_ + m.d_ * v.y_ };
}
