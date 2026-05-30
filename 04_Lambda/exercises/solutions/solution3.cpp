#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <optional>
#include <numeric>

// ============================================================
// Rozwiązanie – Ćwiczenie 3: QueryBuilder<T>
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o solution3 solution3.cpp && ./solution3
// ============================================================

template<typename T>
class QueryBuilder {
    std::vector<T> data_;

public:
    explicit QueryBuilder(std::vector<T> data) : data_(std::move(data)) {}

    QueryBuilder& where(std::function<bool(const T&)> pred) {
        data_.erase(
            std::remove_if(data_.begin(), data_.end(),
                           [&pred](const T& x){ return !pred(x); }),
            data_.end()
        );
        return *this;
    }

    QueryBuilder& order_by(std::function<bool(const T&, const T&)> comp) {
        std::stable_sort(data_.begin(), data_.end(), comp);
        return *this;
    }

    QueryBuilder& transform(std::function<T(T)> f) {
        std::transform(data_.begin(), data_.end(), data_.begin(),
                       [&f](T x){ return f(std::move(x)); });
        return *this;
    }

    QueryBuilder& take(int n) {
        if (n < static_cast<int>(data_.size()))
            data_.resize(static_cast<std::size_t>(n));
        return *this;
    }

    QueryBuilder& skip(int n) {
        int actual = std::min(n, static_cast<int>(data_.size()));
        data_.erase(data_.begin(), data_.begin() + actual);
        return *this;
    }

    std::vector<T> to_vector() const {
        return data_;
    }

    int count() const {
        return static_cast<int>(data_.size());
    }

    std::optional<T> first() const {
        if (data_.empty()) return std::nullopt;
        return data_.front();
    }
};

// ============================================================
// Testy
// ============================================================
struct Student {
    std::string imie;
    int ocena;
    std::string kierunek;

    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        return os << s.imie << "(" << s.ocena << ", " << s.kierunek << ")";
    }
};

std::vector<Student> dane_testowe() {
    return {
        {"Anna",   85, "Informatyka"},
        {"Bartek", 72, "Matematyka"},
        {"Celina", 91, "Informatyka"},
        {"Damian", 65, "Fizyka"},
        {"Ewa",    88, "Informatyka"},
        {"Filip",  79, "Matematyka"},
        {"Grazyna",95, "Fizyka"},
    };
}

void test_where_order() {
    std::cout << "=== where + order_by ===\n";

    auto wynik = QueryBuilder(dane_testowe())
        .where([](const auto& s){ return s.kierunek == "Informatyka"; })
        .where([](const auto& s){ return s.ocena >= 80; })
        .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
        .to_vector();

    std::cout << "Informatycy z ocena >= 80 (malejaco):\n";
    for (const auto& s : wynik) std::cout << "  " << s << "\n";
    // Oczekiwano: Celina(91), Ewa(88), Anna(85)
}

void test_take_skip() {
    std::cout << "\n=== take + skip ===\n";

    auto wynik = QueryBuilder(dane_testowe())
        .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
        .skip(1)   // pomiń najlepszego
        .take(3)   // weź następnych 3
        .to_vector();

    std::cout << "Miejsca 2-4 w rankingu (pominaj 1, bierz 3):\n";
    for (const auto& s : wynik) std::cout << "  " << s << "\n";
}

void test_transform() {
    std::cout << "\n=== transform ===\n";

    // Dodaj 5 punktów premii wszystkim Informatykom
    auto wynik = QueryBuilder(dane_testowe())
        .where([](const auto& s){ return s.kierunek == "Informatyka"; })
        .transform([](Student s){ s.ocena = std::min(100, s.ocena + 5); return s; })
        .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
        .to_vector();

    std::cout << "Informatycy po premii +5:\n";
    for (const auto& s : wynik) std::cout << "  " << s << "\n";
}

void test_count_first() {
    std::cout << "\n=== count + first ===\n";

    auto qb = QueryBuilder(dane_testowe());

    int ile_inf = QueryBuilder(dane_testowe())
        .where([](const auto& s){ return s.kierunek == "Informatyka"; })
        .count();
    std::cout << "Liczba Informatykow: " << ile_inf << "\n";  // 3

    auto najlepszy = QueryBuilder(dane_testowe())
        .order_by([](const auto& a, const auto& b){ return a.ocena > b.ocena; })
        .first();
    std::cout << "Najlepszy: " << (najlepszy ? najlepszy->imie : "brak") << "\n";  // Grazyna

    auto pusty = QueryBuilder(dane_testowe())
        .where([](const auto& s){ return s.ocena > 99; })
        .first();
    std::cout << "Ocena > 99: " << (pusty ? pusty->imie : "brak (nullopt)") << "\n";
}

void test_int_query() {
    std::cout << "\n=== QueryBuilder<int> ===\n";

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto wynik = QueryBuilder(v)
        .where([](int x){ return x % 2 == 0; })    // parzyste
        .transform([](int x){ return x * x; })      // kwadraty
        .order_by([](int a, int b){ return a > b; })// malejąco
        .take(3)                                     // pierwsze 3
        .to_vector();

    std::cout << "Parzyste, kwadrat, malejaco, top3: ";
    for (int x : wynik) std::cout << x << " ";
    std::cout << "\n";  // 100 64 36
}

int main() {
    test_where_order();
    test_take_skip();
    test_transform();
    test_count_first();
    test_int_query();
    std::cout << "\nWszystkie testy zakonczone.\n";
    return 0;
}
