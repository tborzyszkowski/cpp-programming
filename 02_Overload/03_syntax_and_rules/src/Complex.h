#pragma once
#include <iostream>
#include <cmath>
#include <string>

// ============================================================
// Klasa Complex – ilustracja składni przeciążania operatorów
// ============================================================

class Complex {
public:
    Complex(double re = 0.0, double im = 0.0) : re_(re), im_(im) {}

    // ── Metody składowe (lewy operand = *this) ──────────────

    // Arytmetyczne: zwracają nowy obiekt (nie modyfikują *this)
    Complex operator+(const Complex& rhs) const {
        return Complex(re_ + rhs.re_, im_ + rhs.im_);
    }
    Complex operator-(const Complex& rhs) const {
        return Complex(re_ - rhs.re_, im_ - rhs.im_);
    }
    Complex operator*(const Complex& rhs) const {
        return Complex(re_ * rhs.re_ - im_ * rhs.im_,
                       re_ * rhs.im_ + im_ * rhs.re_);
    }

    // Jednoargumentowy – negacja zespolona
    Complex operator-() const { return Complex(-re_, -im_); }
    Complex operator+() const { return *this; }

    // Przypisanie z działaniem: modyfikują *this, zwracają referencję
    Complex& operator+=(const Complex& rhs) {
        re_ += rhs.re_;
        im_ += rhs.im_;
        return *this;
    }
    Complex& operator-=(const Complex& rhs) {
        re_ -= rhs.re_;
        im_ -= rhs.im_;
        return *this;
    }
    Complex& operator*=(const Complex& rhs) {
        *this = *this * rhs;
        return *this;
    }

    // Porównania: zwracają bool
    bool operator==(const Complex& rhs) const {
        return re_ == rhs.re_ && im_ == rhs.im_;
    }
    bool operator!=(const Complex& rhs) const {
        return !(*this == rhs);   // delegacja do ==
    }

    // Gettery
    double re()  const { return re_; }
    double im()  const { return im_; }
    double abs() const { return std::sqrt(re_ * re_ + im_ * im_); }

    // ── Wolne funkcje zaprzyjaźnione ─────────────────────────
    // operator<< i operator>> muszą być wolnymi funkcjami,
    // bo lewy operand to std::ostream / std::istream, nie Complex.
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);

private:
    double re_;
    double im_;
};

// ── Wolna funkcja: skalowanie complex * double ───────────────
// Wolna funkcja umożliwia też: 2.0 * c  (gdyby double konwertował się na Complex)
inline Complex operator*(Complex lhs, double scalar) {
    return Complex(lhs.re() * scalar, lhs.im() * scalar);
}

// ── Definicje wolnych funkcji friend ─────────────────────────
inline std::ostream& operator<<(std::ostream& os, const Complex& c) {
    if (c.im_ >= 0)
        return os << c.re_ << " + " << c.im_ << "i";
    else
        return os << c.re_ << " - " << (-c.im_) << "i";
}

inline std::istream& operator>>(std::istream& is, Complex& c) {
    std::cout << "Re: ";
    is >> c.re_;
    std::cout << "Im: ";
    is >> c.im_;
    return is;
}
