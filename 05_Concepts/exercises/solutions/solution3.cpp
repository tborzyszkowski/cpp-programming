#include <iostream>
#include <concepts>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

// ============================================================
// Rozwiązanie 3: Generyczny Logger z Concepts
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o solution3 solution3.cpp && ./solution3
// ============================================================

// ---- Concepts ----
template<typename T>
concept Drukowalny = requires(T x, std::ostream& os) {
    { os << x } -> std::same_as<std::ostream&>;
};

template<typename T>
concept Serializowalny = requires(T x) {
    { x.serialize() } -> std::convertible_to<std::string>;
};

// ---- Poziomy logowania ----
enum class Poziom { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

std::string nazwa_poziomu(Poziom p) {
    switch (p) {
        case Poziom::DEBUG: return "DEBUG";
        case Poziom::INFO:  return "INFO ";
        case Poziom::WARN:  return "WARN ";
        case Poziom::ERROR: return "ERROR";
    }
    return "?????";
}

// ---- Logger ----
class Logger {
    std::vector<std::function<void(const std::string&)>> sinki_;
    Poziom min_poziom_ = Poziom::DEBUG;
    int    sekwencja_  = 0;
    std::map<Poziom, int> liczniki_;

    std::string formatuj(Poziom p, const std::string& wiadomosc) {
        return "[" + nazwa_poziomu(p) + "] [#" + std::to_string(sekwencja_++) + "] " + wiadomosc;
    }

    void emit(Poziom p, const std::string& wiadomosc) {
        liczniki_[p]++;
        if (p < min_poziom_) return;
        std::string linia = formatuj(p, wiadomosc);
        for (auto& sink : sinki_) sink(linia);
    }

public:
    // Logowanie Drukowalnych (przez operator<<)
    template<Drukowalny T>
    void log(Poziom p, const T& wiadomosc) {
        std::ostringstream oss;
        oss << wiadomosc;
        emit(p, oss.str());
    }

    // Logowanie Serializowalnych (przez serialize())
    template<Serializowalny T>
    void log_obj(Poziom p, const T& obiekt) {
        emit(p, obiekt.serialize());
    }

    // Variadic – łączy wiele części w jeden string
    template<Drukowalny... Ts>
    void log_kontekst(Poziom p, Ts&&... czesci) {
        std::ostringstream oss;
        (oss << ... << std::forward<Ts>(czesci));
        emit(p, oss.str());
    }

    void ustaw_min_poziom(Poziom p) { min_poziom_ = p; }

    void dodaj_wyjscie(std::function<void(const std::string&)> wyjscie) {
        sinki_.push_back(std::move(wyjscie));
    }

    std::map<Poziom, int> statystyki() const { return liczniki_; }
};

// ---- Typy testowe ----
struct Zdarzenie {
    std::string typ;
    int         id;
    std::string serialize() const {
        return "Zdarzenie{typ=" + typ + ",id=" + std::to_string(id) + "}";
    }
};

struct UzytkownikInfo {
    int         id;
    std::string nazwa;
    std::string ip;
    std::string serialize() const {
        return "User{id=" + std::to_string(id) +
               ",nazwa=" + nazwa +
               ",ip=" + ip + "}";
    }
};

// Typ, który nie ma operator<< ani serialize() – celowo:
// struct TajemnicaTyp { int x; };  // NIE skompiluje się z log() lub log_obj()

// ---- Testy ----
int main() {
    std::cout << "=== Generyczny Logger z Concepts ===\n\n";

    Logger logger;
    logger.ustaw_min_poziom(Poziom::INFO);

    // Główny sink – konsola
    logger.dodaj_wyjscie([](const std::string& s) {
        std::cout << s << "\n";
    });

    // Drugi sink – zbieranie do bufora (symulacja pliku)
    std::vector<std::string> bufor;
    logger.dodaj_wyjscie([&bufor](const std::string& s) {
        bufor.push_back(s);
    });

    // Logowanie Drukowalnych:
    logger.log(Poziom::INFO,  std::string{"Aplikacja startuje"});
    logger.log(Poziom::DEBUG, 42);            // poniżej min_poziom – ignorowane
    logger.log(Poziom::WARN,  3.14);
    logger.log(Poziom::ERROR, std::string{"Krytyczny błąd!"});
    logger.log(Poziom::INFO,  true);           // bool jest Drukowalny

    // Logowanie Serializowalnych:
    logger.log_obj(Poziom::INFO,  Zdarzenie{"klik",   1});
    logger.log_obj(Poziom::ERROR, Zdarzenie{"timeout", 2});
    logger.log_obj(Poziom::WARN,  UzytkownikInfo{99, "Jan", "192.168.1.1"});

    // Variadic kontekst:
    logger.log_kontekst(Poziom::INFO, "User ", 99, " zalogował się z ", "127.0.0.1");
    logger.log_kontekst(Poziom::WARN, "Czas odpowiedzi: ", 1500, "ms > próg ", 1000, "ms");

    // Statystyki:
    std::cout << "\n--- Statystyki ---\n";
    auto stats = logger.statystyki();
    for (const auto& [p, n] : stats)
        std::cout << nazwa_poziomu(p) << ": " << n << " logów\n";

    std::cout << "\n--- Bufor (tylko INFO i wyżej) ---\n";
    std::cout << "Przechwycono " << bufor.size() << " wpisów w buforze\n";

    // Zmiana min poziomu i kolejne logi:
    std::cout << "\n--- Po zmianie min na WARN ---\n";
    logger.ustaw_min_poziom(Poziom::WARN);
    logger.log(Poziom::INFO,  std::string{"To nie pojawi się"});
    logger.log(Poziom::WARN,  std::string{"Ostrzeżenie!"});
    logger.log(Poziom::ERROR, std::string{"Błąd!"});

    // Concepts odrzucają nieodpowiednie typy:
    // struct Tajna {};
    // logger.log(Poziom::INFO, Tajna{});       // BŁĄD – nie Drukowalny
    // logger.log_obj(Poziom::INFO, Tajna{});   // BŁĄD – nie Serializowalny
    std::cout << "\n(Nieodpowiednie typy odrzucane w czasie kompilacji – zakomentowane)\n";

    return 0;
}
