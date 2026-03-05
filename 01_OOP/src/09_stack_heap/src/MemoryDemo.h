#pragma once
#include <iostream>
#include <string>
#include <vector>

// =============================================================
// Struktury i klasy do demonstracji stosu i sterty
// =============================================================

struct Point3D {
    double x, y, z;
    int id;

    Point3D(int id, double x = 0, double y = 0, double z = 0)
        : x(x), y(y), z(z), id(id) {
        std::cout << "[Point3D #" << id << "] Konstruktor @ "
                  << this << "\n";
    }
    ~Point3D() {
        std::cout << "[Point3D #" << id << "] Destruktor @ "
                  << this << "\n";
    }
    std::string toString() const {
        return "Point3D#" + std::to_string(id) +
               "(" + std::to_string(x) + "," +
               std::to_string(y) + "," +
               std::to_string(z) + ")";
    }
};

// Klasa demonstrująca wyciek pamięci
class ResourceLeakDemo {
public:
    int* data_;
    std::string label_;

    ResourceLeakDemo(const std::string& label, int size)
        : label_(label), data_(new int[size]) {
        std::cout << "[ResourceLeakDemo] Alloc: " << label_
                  << " @ " << data_ << "\n";
    }

    // BRAK DESTRUKTORA → wyciek pamięci gdy obiekt zniszczony!
    // ~ResourceLeakDemo() { delete[] data_; }  ← BRAKUJE!
};

// Klasa ze POPRAWNYM destruktorem
class ResourceSafeDemo {
public:
    int* data_;
    std::string label_;

    ResourceSafeDemo(const std::string& label, int size)
        : label_(label), data_(new int[size]) {
        std::cout << "[ResourceSafeDemo] Alloc: " << label_
                  << " @ " << data_ << "\n";
    }

    ~ResourceSafeDemo() {
        std::cout << "[ResourceSafeDemo] Free:  " << label_
                  << " @ " << data_ << "\n";
        delete[] data_;    // ← prawidłowe zwolnienie
    }
};
