#pragma once
#include <iostream>

// ============================================================
// Klasa Vec3d – matematyczny wektor 3D
// ============================================================
// Obsługiwane operacje:
//   v1 + v2        – dodawanie wektorów
//   v1 - v2        – odejmowanie wektorów
//   v1 * v2        – iloczyn wektorowy (cross product)
//   v * k, k * v   – skalowanie (mnoienie przez skalar całkowity)
//   ++v, v++       – inkrementacja wszystkich składowych
//   --v, v--       – dekrementacja wszystkich składowych
//   v1 == v2       – porównanie (równość)
//   v1 != v2       – porównanie (nierówność)
//   cout << v      – wyświetlanie: (X: x, Y: y, Z: z)
//   cin  >> v      – wczytywanie składowych ze strumienia
// ============================================================

class Vec3d {
public:
    Vec3d(int x = 0, int y = 0, int z = 0) : x_(x), y_(y), z_(z) {}

    // ── Arytmetyczne ────────────────────────────────────────

    Vec3d operator+(const Vec3d& o) const {
        return Vec3d(x_ + o.x_, y_ + o.y_, z_ + o.z_);
    }
    Vec3d operator-(const Vec3d& o) const {
        return Vec3d(x_ - o.x_, y_ - o.y_, z_ - o.z_);
    }

    // Iloczyn wektorowy (cross product): v1 × v2
    Vec3d operator*(const Vec3d& o) const {
        return Vec3d(
            y_ * o.z_ - z_ * o.y_,
            z_ * o.x_ - x_ * o.z_,
            x_ * o.y_ - y_ * o.x_
        );
    }

    // Skalowanie: v * k
    Vec3d operator*(int k) const {
        return Vec3d(x_ * k, y_ * k, z_ * k);
    }

    // ── Inkrementacja i dekrementacja ───────────────────────

    // Prefix ++v: inkrementuj każdą składową, zwróć referencję
    Vec3d& operator++() {
        ++x_; ++y_; ++z_;
        return *this;
    }
    // Postfix v++: zapamiętaj kopię, inkrementuj, zwróć kopię
    Vec3d operator++(int) {
        Vec3d old = *this;
        ++x_; ++y_; ++z_;
        return old;
    }

    // Prefix --v
    Vec3d& operator--() {
        --x_; --y_; --z_;
        return *this;
    }
    // Postfix v--
    Vec3d operator--(int) {
        Vec3d old = *this;
        --x_; --y_; --z_;
        return old;
    }

    // ── Porównania ──────────────────────────────────────────

    bool operator==(const Vec3d& o) const {
        return x_ == o.x_ && y_ == o.y_ && z_ == o.z_;
    }
    bool operator!=(const Vec3d& o) const { return !(*this == o); }

    // ── Gettery ─────────────────────────────────────────────
    int x() const { return x_; }
    int y() const { return y_; }
    int z() const { return z_; }

    // ── Wejście / wyjście ────────────────────────────────────

    // Wyświetlanie: (X: 1, Y: 2, Z: 3)
    friend std::ostream& operator<<(std::ostream& os, const Vec3d& v) {
        return os << "(X: " << v.x_ << ", Y: " << v.y_ << ", Z: " << v.z_ << ")";
    }

    // Wczytywanie: pyta użytkownika o każdą składową
    friend std::istream& operator>>(std::istream& is, Vec3d& v) {
        std::cout << "X: "; is >> v.x_;
        std::cout << "Y: "; is >> v.y_;
        std::cout << "Z: "; is >> v.z_;
        return is;
    }

private:
    int x_;
    int y_;
    int z_;
};

// Skalowanie z lewej strony: k * v  (nie może być metodą – lewy operand to int)
inline Vec3d operator*(int k, const Vec3d& v) {
    return v * k;
}
