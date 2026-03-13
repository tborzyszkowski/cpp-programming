#pragma once
#include <iostream>
#include <string>

// Forward declarations
class Workshop;
class ServiceReport;

// =============================================================
// Klasa Car – demonstruje zaprzyjaźnienie (friend)
// =============================================================
class Car {
private:
    std::string brand_;
    std::string model_;
    int         year_;
    double      mileage_;
    double      fuelLevel_;   // 0.0 .. 100.0

public:
    Car(const std::string& brand, const std::string& model,
        int year, double mileage = 0.0, double fuelLevel = 80.0)
        : brand_(brand), model_(model), year_(year),
          mileage_(mileage), fuelLevel_(fuelLevel) {}

    // Publiczne metody
    std::string brand()    const { return brand_; }
    std::string model()    const { return model_; }
    int         year()     const { return year_;   }
    double      mileage()  const { return mileage_; }

    std::string toString() const {
        return brand_ + " " + model_ + " (" + std::to_string(year_) + ")"
               + " [km=" + std::to_string(static_cast<int>(mileage_))
               + ", fuel=" + std::to_string(static_cast<int>(fuelLevel_)) + "%]";
    }

    // --- ZAPRZYJAŹNIONA FUNKCJA ---
    // Może dostępować PRYWATNYCH pól brand_, model_, fuelLevel_ itd.
    friend void printServiceReport(const Car& car, double cost);

    // --- ZAPRZYJAŹNIONA KLASA ---
    // Wszystkie metody klasy Workshop mają dostęp do prywatnych pól Car
    friend class Workshop;
};

// =============================================================
// Zaprzyjaźniona funkcja (friend function) – zdefiniowana poza klasą
// Ma dostęp do prywatnych pól Car!
// =============================================================
void printServiceReport(const Car& car, double cost) {
    // Dostęp do PRYWATNYCH pól:
    std::cout << "=== Raport Serwisowy ===\n";
    std::cout << "Pojazd: " << car.brand_ << " " << car.model_
              << " (" << car.year_ << ")\n";
    std::cout << "Przebieg: " << car.mileage_ << " km\n";    // prywatne!
    std::cout << "Paliwo: "   << car.fuelLevel_ << "%\n";    // prywatne!
    std::cout << "Koszt usługi: " << cost << " PLN\n";
    std::cout << "========================\n";
}

// =============================================================
// Zaprzyjaźniona klasa Workshop
// WSZYSTKIE jej metody mają dostęp do prywatnych pól Car
// =============================================================
class Workshop {
private:
    std::string name_;
    double      hourlyRate_;

public:
    Workshop(const std::string& name, double rate)
        : name_(name), hourlyRate_(rate) {}

    // Metoda serwisu – może modyfikować prywatne pola Car
    void service(Car& car, double hoursWorked) {
        double cost = hoursWorked * hourlyRate_;
        // Dostęp do PRYWATNYCH pól Car:
        car.mileage_   += 0;        // nie zmienia przebiegu
        car.fuelLevel_  = 100.0;    // uzupełnij paliwo (prywatne!)
        std::cout << "[Workshop: " << name_ << "] Serwis "
                  << car.brand_ << " " << car.model_    // prywatne!
                  << ", czas: " << hoursWorked << "h, koszt: " << cost << " PLN\n";
        std::cout << "  Uzupełniono paliwo → " << car.fuelLevel_ << "%\n";
    }

    // Diagnostyka – tylko odczyt prywatnych pól
    void diagnose(const Car& car) {
        std::cout << "[Workshop: " << name_ << "] Diagnostyka:\n";
        std::cout << "  Marka/Model: " << car.brand_ << " " << car.model_ << "\n";
        std::cout << "  Rok: "         << car.year_      << "\n";
        std::cout << "  Przebieg: "    << car.mileage_   << " km\n";
        std::cout << "  Paliwo: "      << car.fuelLevel_ << "%\n";
    }
};
