#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

class Temperature {
    double kelvin_;

    void validate() const {
        if (kelvin_ < 0.0)
            throw std::invalid_argument("Temperatura nie może być poniżej 0 K");
    }

public:
    explicit Temperature(double kelvin) : kelvin_(kelvin) { validate(); }

    // --- fabryki ---
    static Temperature fromCelsius(double c) { return Temperature(c + 273.15); }
    static Temperature fromFahrenheit(double f) { return Temperature((f - 32.0) * 5.0/9.0 + 273.15); }

    // --- konwersje ---
    double celsius() const { return kelvin_ - 273.15; }
    double fahrenheit() const { return (kelvin_ - 273.15) * 9.0/5.0 + 32.0; }
    explicit operator double() const { return kelvin_; }

    // --- arytmetyka ---
    double operator-(const Temperature& rhs) const { return kelvin_ - rhs.kelvin_; }

    Temperature operator+(double delta) const { return Temperature(kelvin_ + delta); }
    Temperature operator-(double delta) const { return Temperature(kelvin_ - delta); }

    Temperature& operator+=(double delta) { kelvin_ += delta; validate(); return *this; }
    Temperature& operator-=(double delta) { kelvin_ -= delta; validate(); return *this; }

    friend Temperature operator+(double delta, const Temperature& t) { return t + delta; }

    // --- porównanie (z tolerancją) ---
    bool operator==(const Temperature& rhs) const {
        return std::abs(kelvin_ - rhs.kelvin_) < 1e-9;
    }
    bool operator!=(const Temperature& rhs) const { return !(*this == rhs); }
    bool operator<(const Temperature& rhs)  const { return kelvin_ < rhs.kelvin_ - 1e-9; }
    bool operator>(const Temperature& rhs)  const { return rhs < *this; }
    bool operator<=(const Temperature& rhs) const { return !(rhs < *this); }
    bool operator>=(const Temperature& rhs) const { return !(*this < rhs); }

    // --- strumień ---
    friend std::ostream& operator<<(std::ostream& os, const Temperature& t) {
        os << std::fixed << std::setprecision(2)
           << t.kelvin_ << " K (" << t.celsius() << "°C)";
        return os;
    }
};

// --- literały użytkownika ---
namespace temperature_literals {
    Temperature operator""_K(long double val) { return Temperature(static_cast<double>(val)); }
    Temperature operator""_C(long double val) { return Temperature::fromCelsius(static_cast<double>(val)); }
    Temperature operator""_F(long double val) { return Temperature::fromFahrenheit(static_cast<double>(val)); }
}
