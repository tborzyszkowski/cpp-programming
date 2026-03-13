#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>

// =============================================================
// Klasa Circle ilustruje stałe metody, stałe obiekty i const
// =============================================================
class Circle {
private:
    double radius_;
    std::string color_;

public:
    Circle(double radius, const std::string& color = "red")
        : radius_(radius), color_(color) {
        std::cout << "[Circle] Konstruktor: r=" << radius_ << "\n";
    }

    // --- Gettery – powinny być CONST (nie modyfikują obiektu) ---
    double radius() const { return radius_; }
    std::string color() const { return color_; }

    // --- Metody obliczeniowe – CONST (tylko czytają stan) ---
    double area() const {
        return PI * radius_ * radius_;
    }
    double circumference() const {
        return 2.0 * PI * radius_;
    }

    // --- Setter – NIE const (modyfikuje obiekt) ---
    void setRadius(double r) {
        if (r <= 0) throw std::invalid_argument("Promień musi być > 0");
        radius_ = r;
    }
    void setColor(const std::string& c) { color_ = c; }

    // Przeciążona metoda toString: callable zarówno na const i non-const
    std::string toString() const {
        return "Circle(r=" + std::to_string(radius_) +
               ", color=" + color_ + ")";
    }

    // Metoda skalowania – MODYFIKUJE obiekt, więc NIE jest const
    void scale(double factor) {
        if (factor <= 0) throw std::invalid_argument("Factor musi być > 0");
        radius_ *= factor;
    }

    // --- static constexpr – stała czasu kompilacji ---
    static constexpr double PI = 3.14159265358979323846;

    // Wersja z użyciem własnego PI
    double areaAlt() const {
        return PI * radius_ * radius_;
    }
};

// --- Funkcja przyjmująca obiekt przez const ref ---
// Może wywołać TYLKO metody const!
void printCircleInfo(const Circle& c) {
    std::cout << c.toString() << "\n";
    std::cout << "  area          = " << c.area()          << "\n";
    std::cout << "  circumference = " << c.circumference() << "\n";
    // c.setRadius(5.0);  // BŁĄD KOMPILACJI – setRadius nie jest const!
}
