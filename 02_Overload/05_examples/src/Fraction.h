#pragma once
#include <iostream>
#include <numeric>    // std::gcd (C++17)
#include <stdexcept>
#include <cstdlib>    // std::abs

// ============================================================
// Klasa Fraction – ułamek zwykły z pełnym zestawem operatorów
// ============================================================
// Niezmiennik (invariant):
//   - den_ > 0  (znak przechowywany w liczniku)
//   - gcd(|num_|, den_) == 1  (postać nieskracalna)
// ============================================================

class Fraction {
public:
    // Konstruktor: automatycznie normalizuje ułamek
    Fraction(int num = 0, int den = 1) : num_(num), den_(den) {
        if (den_ == 0)
            throw std::invalid_argument("Mianownik nie moze byc zerem!");
        normalize();
    }

    // ── Arytmetyczne – zwracają nowy obiekt ─────────────────

    Fraction operator+(const Fraction& o) const {
        return Fraction(num_ * o.den_ + o.num_ * den_, den_ * o.den_);
    }
    Fraction operator-(const Fraction& o) const {
        return Fraction(num_ * o.den_ - o.num_ * den_, den_ * o.den_);
    }
    Fraction operator*(const Fraction& o) const {
        return Fraction(num_ * o.num_, den_ * o.den_);
    }
    Fraction operator/(const Fraction& o) const {
        if (o.num_ == 0)
            throw std::domain_error("Dzielenie przez zero (ulamek = 0)!");
        return Fraction(num_ * o.den_, den_ * o.num_);
    }

    // ── Jednoargumentowe ─────────────────────────────────────
    Fraction operator-() const { return Fraction(-num_, den_); }
    Fraction operator+() const { return *this; }

    // ── Przypisanie z działaniem – delegują do operatorów arytm. ──
    Fraction& operator+=(const Fraction& o) { *this = *this + o; return *this; }
    Fraction& operator-=(const Fraction& o) { *this = *this - o; return *this; }
    Fraction& operator*=(const Fraction& o) { *this = *this * o; return *this; }
    Fraction& operator/=(const Fraction& o) { *this = *this / o; return *this; }

    // ── Porównania ────────────────────────────────────────────
    // == działa, bo ułamki są znormalizowane (kanoniczna forma)
    bool operator==(const Fraction& o) const { return num_ == o.num_ && den_ == o.den_; }
    bool operator!=(const Fraction& o) const { return !(*this == o); }

    // Porównanie przez sprowadzenie do wspólnego mianownika
    bool operator< (const Fraction& o) const { return num_ * o.den_ < o.num_ * den_; }
    bool operator> (const Fraction& o) const { return o < *this; }
    bool operator<=(const Fraction& o) const { return !(o < *this); }
    bool operator>=(const Fraction& o) const { return !(*this < o); }

    // ── Gettery ───────────────────────────────────────────────
    int    numerator()   const { return num_; }
    int    denominator() const { return den_; }
    double toDouble()    const { return static_cast<double>(num_) / den_; }

    // ── Wolne funkcje zaprzyjaźnione (<<, >>) ────────────────
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::istream& operator>>(std::istream& is, Fraction& f);

private:
    int num_;
    int den_;

    // Normalizacja: sign in numerator + skrócenie przez NWD
    void normalize() {
        if (den_ < 0) { num_ = -num_; den_ = -den_; }
        int g = std::gcd(std::abs(num_), den_);
        num_ /= g;
        den_ /= g;
    }
};

// ── Wyjście: 3/4 lub 2 (gdy den_==1) ─────────────────────────
inline std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    if (f.den_ == 1)
        return os << f.num_;
    return os << f.num_ << "/" << f.den_;
}

// ── Wejście: oczekuje formatu liczba/liczba (np. 3/4) ─────────
inline std::istream& operator>>(std::istream& is, Fraction& f) {
    int n, d;
    char slash;
    if (is >> n >> slash >> d)
        f = Fraction(n, d);
    return is;
}
