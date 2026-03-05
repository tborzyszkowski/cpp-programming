#pragma once
#include "Point.h"
#include <string>
#include <iostream>
#include <cmath>

// Klasa Rectangle – prostokąt zdefiniowany przez dwa punkty
class Rectangle {
private:
    Point topLeft_;
    Point bottomRight_;
    std::string color_;

public:
    // Konstruktor przyjmujący dwa punkty i kolor
    Rectangle(const Point& topLeft, const Point& bottomRight,
              const std::string& color = "white")
        : topLeft_(topLeft), bottomRight_(bottomRight), color_(color) {
        std::cout << "[Rectangle] Konstruktor: " << toString() << "\n";
    }

    // Obliczenia geometryczne
    double width()  const { return std::abs(bottomRight_.x() - topLeft_.x()); }
    double height() const { return std::abs(bottomRight_.y() - topLeft_.y()); }
    double area()   const { return width() * height(); }
    double perimeter() const { return 2.0 * (width() + height()); }

    // Getter i setter koloru
    std::string color() const { return color_; }
    void setColor(const std::string& c) { color_ = c; }

    std::string toString() const {
        return "Rectangle[" + topLeft_.toString() + " -> " +
               bottomRight_.toString() + ", color=" + color_ +
               ", area=" + std::to_string(area()) + "]";
    }

    ~Rectangle() {
        std::cout << "[Rectangle] Destruktor: " << toString() << "\n";
    }
};
