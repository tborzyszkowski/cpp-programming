#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include <map>
#include <sstream>
#include <cmath>
#include <future>
#include <memory>

// ============================================================
// Demonstracja: Zastosowania praktyczne lambd
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o practical main.cpp && ./practical
// ============================================================

// ============================================================
// 1. Zaawansowane algorytmy STL
// ============================================================
void demo_stl() {
    std::cout << "=== STL z lambdami ===\n";

    std::vector<std::pair<std::string, int>> pracownicy = {
        {"Anna", 85}, {"Bartek", 92}, {"Celina", 78}, {"Damian", 92}, {"Ewa", 65}
    };

    // Sortowanie: najpierw wynik malejąco, potem imię rosnąco
    std::sort(pracownicy.begin(), pracownicy.end(),
        [](const auto& a, const auto& b){
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

    std::cout << "Ranking:\n";
    int miejsce = 1;
    for (const auto& [name, score] : pracownicy)
        std::cout << "  " << miejsce++ << ". " << name << " (" << score << ")\n";

    // Partycjonowanie: zdali (>= 70) i nie zdali
    auto granica = std::stable_partition(pracownicy.begin(), pracownicy.end(),
        [](const auto& p){ return p.second >= 70; });
    std::cout << "Zdali: " << std::distance(pracownicy.begin(), granica) << "\n";

    // transform_reduce (C++17): suma ważona
    double srednia = std::transform_reduce(
        pracownicy.begin(), pracownicy.end(),
        0.0, std::plus<>{},
        [](const auto& p){ return static_cast<double>(p.second); }
    ) / pracownicy.size();
    std::cout << "Srednia: " << srednia << "\n";
}

// ============================================================
// 2. System callbacków / zdarzeń
// ============================================================
class EventEmitter {
    std::map<std::string, std::vector<std::function<void(const std::string&)>>> handlers_;
public:
    void on(const std::string& event, std::function<void(const std::string&)> handler) {
        handlers_[event].push_back(std::move(handler));
    }

    void emit(const std::string& event, const std::string& data = "") {
        auto it = handlers_.find(event);
        if (it != handlers_.end())
            for (auto& h : it->second) h(data);
    }
};

void demo_callbacki() {
    std::cout << "\n=== Callbacki / system zdarzen ===\n";

    EventEmitter emitter;

    int logCount = 0;
    emitter.on("login",  [&logCount](const std::string& user){
        ++logCount;
        std::cout << "Zalogowano: " << user << " (lacznie: " << logCount << ")\n";
    });
    emitter.on("login",  [](const std::string& user){
        std::cout << "Wyslano e-mail powitalny do: " << user << "\n";
    });
    emitter.on("logout", [](const std::string& user){
        std::cout << "Wylogowano: " << user << "\n";
    });

    emitter.emit("login", "Anna");
    emitter.emit("login", "Bartek");
    emitter.emit("logout", "Anna");
}

// ============================================================
// 3. Wzorzec Strategy bez hierarchii klas
// ============================================================
class DataPipeline {
    std::vector<std::function<std::vector<int>(std::vector<int>)>> etapy_;
public:
    DataPipeline& dodaj(std::function<std::vector<int>(std::vector<int>)> etap) {
        etapy_.push_back(std::move(etap));
        return *this;
    }

    std::vector<int> uruchom(std::vector<int> dane) const {
        for (const auto& etap : etapy_)
            dane = etap(std::move(dane));
        return dane;
    }
};

void demo_strategy() {
    std::cout << "\n=== Wzorzec Strategy (pipeline) ===\n";

    DataPipeline pipeline;
    pipeline
        .dodaj([](auto v){  // Filtruj: tylko parzyste
            v.erase(std::remove_if(v.begin(), v.end(),
                    [](int x){ return x % 2 != 0; }), v.end());
            return v;
        })
        .dodaj([](auto v){  // Transformuj: potrój
            std::transform(v.begin(), v.end(), v.begin(),
                           [](int x){ return x * 3; });
            return v;
        })
        .dodaj([](auto v){  // Sortuj malejąco
            std::sort(v.begin(), v.end(), std::greater<int>{});
            return v;
        });

    auto wynik = pipeline.uruchom({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::cout << "Wynik: ";
    for (int x : wynik) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 4. Mini framework testowy z lambdami (DSL)
// ============================================================
struct Test {
    std::string nazwa;
    std::function<bool()> sprawdz;
};

void uruchom_testy(const std::vector<Test>& testy) {
    int ok = 0, fail = 0;
    std::cout << "\n=== Testy jednostkowe ===\n";
    for (const auto& t : testy) {
        bool wynik = t.sprawdz();
        std::cout << (wynik ? "[OK]   " : "[FAIL] ") << t.nazwa << "\n";
        wynik ? ++ok : ++fail;
    }
    std::cout << ok << "/" << (ok + fail) << " zaliczonych\n";
}

void demo_testy() {
    uruchom_testy({
        {"2 + 2 == 4",
            []{ return (2 + 2) == 4; }},
        {"sort rosnaco",
            []{
                std::vector<int> v = {3, 1, 4, 1, 5, 9};
                std::sort(v.begin(), v.end());
                return std::is_sorted(v.begin(), v.end());
            }},
        {"count_if parzyste",
            []{
                std::vector<int> v = {1, 2, 3, 4, 5, 6};
                return std::count_if(v.begin(), v.end(),
                       [](int x){ return x % 2 == 0; }) == 3;
            }},
        {"string trim (symulacja)",
            []{
                std::string s = "  hello  ";
                s.erase(0, s.find_first_not_of(' '));
                s.erase(s.find_last_not_of(' ') + 1);
                return s == "hello";
            }},
        {"Falszywy test (demonstracja FAIL)",
            []{ return false; }},
    });
}

// ============================================================
// 5. Analiza danych finansowych
// ============================================================
void demo_finanse() {
    std::cout << "\n=== Analiza danych finansowych ===\n";

    struct Transakcja { std::string id; double kwota; std::string typ; };

    std::vector<Transakcja> transakcje = {
        {"T001",  150.0, "credit"},
        {"T002", -50.0,  "debit"},
        {"T003",  200.0, "credit"},
        {"T004", -30.0,  "debit"},
        {"T005",  75.0,  "credit"},
        {"T006", -120.0, "debit"},
    };

    // Saldo
    double saldo = std::accumulate(transakcje.begin(), transakcje.end(), 0.0,
        [](double acc, const Transakcja& t){ return acc + t.kwota; });
    std::cout << "Saldo: " << saldo << "\n";

    // Suma wpłat i wypłat osobno
    auto suma_kredytow = std::accumulate(transakcje.begin(), transakcje.end(), 0.0,
        [](double acc, const Transakcja& t){
            return acc + (t.typ == "credit" ? t.kwota : 0.0);
        });
    auto suma_debetow = std::accumulate(transakcje.begin(), transakcje.end(), 0.0,
        [](double acc, const Transakcja& t){
            return acc + (t.typ == "debit" ? std::abs(t.kwota) : 0.0);
        });
    std::cout << "Wplaty: +" << suma_kredytow << "\n";
    std::cout << "Wyplaty: -" << suma_debetow << "\n";

    // Transakcje powyżej progu
    double prog = 100.0;
    std::cout << "Transakcje |kwota| > " << prog << ": ";
    for (const auto& t : transakcje)
        if (std::abs(t.kwota) > prog)
            std::cout << t.id << "(" << t.kwota << ") ";
    std::cout << "\n";
}

// ============================================================
// 6. IIFE dla inicjalizacji złożonych stałych
// ============================================================
void demo_iife() {
    std::cout << "\n=== IIFE – inicjalizacja stalych ===\n";

    // Tablica pierwszych n liczb pierwszych
    const auto pierwsze = []{
        std::vector<int> p;
        for (int n = 2; p.size() < 10; ++n) {
            bool jest = true;
            for (int d = 2; d * d <= n; ++d)
                if (n % d == 0) { jest = false; break; }
            if (jest) p.push_back(n);
        }
        return p;
    }();

    std::cout << "Pierwsze 10 liczb pierwszych: ";
    for (int p : pierwsze) std::cout << p << " ";
    std::cout << "\n";

    // Mapa kodów błędów (inicjalizacja warunkowa)
    const std::map<int, std::string> kody_http = []{
        std::map<int, std::string> m;
        for (auto [kod, opis] : std::initializer_list<std::pair<int,const char*>>{
            {200,"OK"},{201,"Created"},{400,"Bad Request"},
            {401,"Unauthorized"},{403,"Forbidden"},{404,"Not Found"},
            {500,"Internal Server Error"}
        }) m[kod] = opis;
        return m;
    }();

    for (int kod : {200, 404, 500})
        std::cout << "HTTP " << kod << ": " << kody_http.at(kod) << "\n";
}

int main() {
    demo_stl();
    demo_callbacki();
    demo_strategy();
    demo_testy();
    demo_finanse();
    demo_iife();
    return 0;
}
