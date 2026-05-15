#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <optional>
#include <variant>
#include <any>
#include <chrono>
#include <algorithm>

// ============================================================
// Demonstracja: Narzędzia ogólne C++17
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o utilities main.cpp && ./utilities
// ============================================================

// --- optional: funkcja która może nie zwrócić wartości ------
std::optional<int> podziel(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

std::optional<std::string> znajdz_uzytkownika(int id) {
    if (id == 1) return "Anna";
    if (id == 2) return "Bartek";
    return std::nullopt;
}

void demo_optional() {
    std::cout << "=== std::optional ===\n";

    auto r1 = podziel(10, 3);
    auto r2 = podziel(10, 0);

    std::cout << "10/3 = " << r1.value_or(-1) << "\n";  // 3
    std::cout << "10/0 = " << r2.value_or(-1) << "\n";  // -1

    for (int id : {1, 2, 3}) {
        auto user = znajdz_uzytkownika(id);
        if (user)
            std::cout << "ID " << id << ": " << *user << "\n";
        else
            std::cout << "ID " << id << ": nie znaleziono\n";
    }
}

// --- variant: bezpieczna unia ---
using Wynik = std::variant<int, double, std::string>;

Wynik oblicz(const std::string& op) {
    if (op == "int")    return 42;
    if (op == "double") return 3.14;
    return std::string{"błąd"};
}

void demo_variant() {
    std::cout << "\n=== std::variant ===\n";

    for (const auto& op : {"int", "double", "unknown"}) {
        auto w = oblicz(op);
        std::visit([](const auto& val){
            std::cout << "Wynik: " << val << "\n";
        }, w);
    }

    // holds_alternative
    Wynik v = 42;
    std::cout << "Jest int: " << std::boolalpha
        << std::holds_alternative<int>(v) << "\n";

    v = 3.14;
    std::cout << "Jest double: " << std::holds_alternative<double>(v) << "\n";
}

void demo_any() {
    std::cout << "\n=== std::any ===\n";

    std::any a = 42;
    std::cout << std::any_cast<int>(a) << "\n";

    a = std::string{"Hello"};
    if (auto* p = std::any_cast<std::string>(&a))
        std::cout << *p << "\n";

    try {
        std::any_cast<int>(a);
    } catch (const std::bad_any_cast& e) {
        std::cout << "bad_any_cast złapany\n";
    }
}

void demo_tuple() {
    std::cout << "\n=== std::pair i std::tuple ===\n";

    auto p = std::make_pair("Anna", 30);
    auto [imie, wiek] = p;  // structured binding C++17
    std::cout << imie << ", " << wiek << "\n";

    auto t = std::make_tuple("Bartek", 25, 180.5);
    auto [n, a, h] = t;
    std::cout << n << ", " << a << ", " << h << "\n";

    // Sortowanie par (domyślnie leksykograficzne)
    std::vector<std::pair<int, std::string>> v = {
        {3, "C"}, {1, "A"}, {2, "B"}};
    std::sort(v.begin(), v.end());
    for (const auto& [k, s] : v)
        std::cout << k << ":" << s << " ";
    std::cout << "\n";
}

void demo_chrono() {
    std::cout << "\n=== std::chrono ===\n";
    using namespace std::chrono;

    auto start = steady_clock::now();

    // Symulacja pracy
    volatile long suma = 0;
    for (long i = 0; i < 5'000'000; ++i) suma += i;

    auto stop = steady_clock::now();
    auto ms = duration_cast<milliseconds>(stop - start).count();
    auto us = duration_cast<microseconds>(stop - start).count();

    std::cout << "Czas: " << ms << " ms (" << us << " µs)\n";
    std::cout << "Suma: " << suma << "\n";

    // Literały czasowe (C++14)
    auto deadline = 1h + 30min + 45s;
    std::cout << "Deadline: "
        << duration_cast<seconds>(deadline).count() << " s\n";
}

int main() {
    demo_optional();
    demo_variant();
    demo_any();
    demo_tuple();
    demo_chrono();
    return 0;
}
