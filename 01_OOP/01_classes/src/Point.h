#pragma once
#include <string>
#include <iostream>
#include <cmath>

// Definicja klasy Point - reprezentuje punkt na płaszczyźnie 2D
class Point {
private:
    // Pola (atrybuty) – dane przechowywane w każdym obiekcie
    double x_;
    double y_;

public:
    // Konstruktor domyślny (zero-arg)
    Point() : x_(0.0), y_(0.0) {
        std::cout << "[Point] Konstruktor domyślny: (0, 0)\n";
    }

    // Konstruktor parametryczny
    Point(double x, double y) : x_(x), y_(y) {
        std::cout << "[Point] Konstruktor: (" << x_ << ", " << y_ << ")\n";
    }

    // Destruktor
    ~Point() {
        std::cout << "[Point] Destruktor: (" << x_ << ", " << y_ << ")\n";
    }

    // Metody dostępowe (gettery) – const: nie modyfikują obiektu
    double x() const { return x_; }
    double y() const { return y_; }

    // Settery
    void setX(double x) { x_ = x; }
    void setY(double y) { y_ = y; }

    // Obliczenie odległości od początku układu współrzędnych
    double distanceFromOrigin() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    // Reprezentacja tekstowa obiektu
    std::string toString() const {
        return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
    }
};
