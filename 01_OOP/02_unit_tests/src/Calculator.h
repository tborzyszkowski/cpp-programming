#pragma once
#include <stdexcept>
#include <string> 
// prosta implementacja do demonstracji testów jednostkowych
class Calculator {
public:
    // Dodawanie
    double add(double a, double b) const {
        return a + b;
    }

    // Odejmowanie
    double subtract(double a, double b) const {
        return a - b;
    }

    // Mnożenie
    double multiply(double a, double b) const {
        return a * b;
    }

    // Dzielenie – rzuca wyjątek dla dzielnika 0
    double divide(double a, double b) const {
        if (b == 0.0) {
            throw std::invalid_argument("Dzielnik nie może być zerem!");
        }
        return a / b;
    }

    // Sprawdzenie czy liczba jest parzysta
    bool isEven(int n) const {
        return n % 2 == 0;
    }

    // Silnia (rekurencja)
    long long factorial(int n) const {
        if (n < 0) throw std::invalid_argument("Silnia tylko dla n >= 0");
        if (n == 0 || n == 1) return 1;
        return n * factorial(n - 1);
    }
};
