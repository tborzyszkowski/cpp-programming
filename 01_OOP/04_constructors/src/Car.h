#pragma once
#include <string>
#include <iostream>

// =============================================================
// Klasa Car demonstrująca wszystkie rodzaje konstruktorów i destruktor
// =============================================================
class Car {
private:
    std::string brand_;
    std::string model_;
    int         year_;
    double      fuelConsumption_;   // l/100km
    int*        serviceHistory_;    // dynamiczna tablica – liczba przeglądów
    int         serviceCount_;

public:
    // --- 1. Konstruktor domyślny (zero-arg) ---
    Car() : brand_("Unknown"), model_("Unknown"), year_(2000),
            fuelConsumption_(0.0), serviceCount_(0) {
        serviceHistory_ = new int[10]{};
        std::cout << "[Car] Konstruktor domyślny\n";
    }

    // --- 2. Konstruktor parametryczny ---
    Car(const std::string& brand, const std::string& model,
        int year, double fuelConsumption = 0.0)
        : brand_(brand), model_(model), year_(year),
          fuelConsumption_(fuelConsumption), serviceCount_(0) {
        serviceHistory_ = new int[10]{};
        std::cout << "[Car] Konstruktor: " << toString() << "\n";
    }

    // --- 3. Konstruktor delegujący (C++11) ---
    Car(const std::string& brand, const std::string& model)
        : Car(brand, model, 2024) {    // deleguje do konstruktora 4-arg
        std::cout << "[Car] Konstruktor 2-arg (delegujący)\n";
    }

    // --- 4. Konstruktor kopiujący (deep copy!) ---
    Car(const Car& other)
        : brand_(other.brand_), model_(other.model_),
          year_(other.year_), fuelConsumption_(other.fuelConsumption_),
          serviceCount_(other.serviceCount_) {
        serviceHistory_ = new int[10]{};
        std::copy(other.serviceHistory_, other.serviceHistory_ + 10,
                  serviceHistory_);
        std::cout << "[Car] Konstruktor kopiujący: " << toString() << "\n";
    }

    // --- 5. Operator przypisania kopiującego ---
    Car& operator=(const Car& other) {
        if (this == &other) return *this;
        brand_           = other.brand_;
        model_           = other.model_;
        year_            = other.year_;
        fuelConsumption_ = other.fuelConsumption_;
        serviceCount_    = other.serviceCount_;
        delete[] serviceHistory_;
        serviceHistory_ = new int[10]{};
        std::copy(other.serviceHistory_, other.serviceHistory_ + 10,
                  serviceHistory_);
        std::cout << "[Car] operator= (kopiujący)\n";
        return *this;
    }

    // --- 6. Destruktor ---
    ~Car() {
        std::cout << "[Car] Destruktor: " << brand_ << " " << model_ << "\n";
        delete[] serviceHistory_;    // zwolnij dynamiczną pamięć
    }

    // Dodanie przeglądu
    void addService(int year) {
        if (serviceCount_ < 10)
            serviceHistory_[serviceCount_++] = year;
    }

    // Akcesory
    std::string brand()          const { return brand_;          }
    std::string model()          const { return model_;          }
    int         year()           const { return year_;           }
    double      fuelConsumption() const { return fuelConsumption_; }

    std::string toString() const {
        return brand_ + " " + model_ + " (" + std::to_string(year_) + ")";
    }
};
