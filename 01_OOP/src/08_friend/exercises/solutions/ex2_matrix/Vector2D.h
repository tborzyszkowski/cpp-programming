#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

// Forward declaration – Vector2D odwołuje się do Matrix2x2 i odwrotnie
class Matrix2x2;

// =============================================================
// Klasa Vector2D
// =============================================================
class Vector2D {
private:
    double x_;
    double y_;

public:
    Vector2D(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}

    double length() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    Vector2D operator+(const Vector2D& other) const {
        return {x_ + other.x_, y_ + other.y_};
    }

    Vector2D operator*(double scalar) const {
        return {x_ * scalar, y_ * scalar};
    }

    // ---- zaprzyjaźnienia ----
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

    // operator* (Matrix2x2 × Vector2D) musi mieć dostęp do x_, y_
    friend Vector2D operator*(const Matrix2x2& m, const Vector2D& v);
};

inline std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << std::fixed << std::setprecision(2)
       << "(" << v.x_ << ", " << v.y_ << ")";
    return os;
}
