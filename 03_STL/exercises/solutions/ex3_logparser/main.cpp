#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct LogEntry {
    std::string date;
    std::string time;
    std::string level;
    std::string message;
};

class LogReport {
    std::vector<LogEntry> wpisy_;
public:
    void add_entry(const LogEntry& e) { wpisy_.push_back(e); }

    std::map<std::string, int> count_by_level() const {
        std::map<std::string, int> wynik;
        for (const auto& e : wpisy_)
            ++wynik[e.level];
        return wynik;
    }

    std::vector<std::string> error_days() const {
        std::set<std::string> dni;
        for (const auto& e : wpisy_)
            if (e.level == "ERROR")
                dni.insert(e.date);
        return {dni.begin(), dni.end()};
    }

    std::vector<std::string> search(std::string_view fragment) const {
        std::vector<std::string> wynik;
        for (const auto& e : wpisy_)
            if (std::string_view{e.message}.find(fragment) != std::string_view::npos)
                wynik.push_back(e.message);
        return wynik;
    }

    friend std::ostream& operator<<(std::ostream& os, const LogReport& r) {
        os << "=== Raport logów ===\n";
        os << "Wpisy wg poziomu:\n";
        for (const auto& [level, cnt] : r.count_by_level())
            os << "  " << level << ": " << cnt << "\n";
        os << "Dni z błędami:\n";
        for (const auto& d : r.error_days())
            os << "  " << d << "\n";
        return os;
    }
};

// Parsuje linię w formacie: "2024-01-15 10:23:45 INFO  Treść komunikatu"
std::optional<LogEntry> parse_line(const std::string& line) {
    std::istringstream iss(line);
    LogEntry e;
    if (!(iss >> e.date >> e.time >> e.level)) return std::nullopt;
    std::getline(iss, e.message);
    // Usuń wiodące spacje z wiadomości
    auto start = e.message.find_first_not_of(' ');
    if (start != std::string::npos)
        e.message = e.message.substr(start);
    return e;
}

LogReport parse_log(const std::string& filename) {
    LogReport raport;
    std::ifstream plik(filename);
    if (!plik.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << "\n";
        return raport;
    }
    std::string linia;
    while (std::getline(plik, linia)) {
        if (linia.empty()) continue;
        if (auto e = parse_line(linia))
            raport.add_entry(*e);
    }
    return raport;
}

// Demo z danymi w pamięci (bez pliku)
LogReport create_demo_report() {
    const std::vector<std::string> linie = {
        "2024-01-15 10:23:45 INFO  Serwer uruchomiony",
        "2024-01-15 10:24:01 ERROR Błąd połączenia z bazą",
        "2024-01-15 10:24:10 WARN  Wysoki CPU",
        "2024-01-16 09:00:00 INFO  Backup wykonany",
        "2024-01-16 09:15:30 ERROR Dysk pełny",
        "2024-01-17 08:00:00 INFO  Restart usługi",
    };
    LogReport raport;
    for (const auto& l : linie)
        if (auto e = parse_line(l))
            raport.add_entry(*e);
    return raport;
}

int main() {
    auto raport = create_demo_report();
    std::cout << raport;

    std::cout << "\nWyszukiwanie 'błąd':\n";
    for (const auto& msg : raport.search("błąd"))
        std::cout << "  - " << msg << "\n";

    std::cout << "\nWyszukiwanie 'INFO'... (brak, bo szukamy w treści)\n";
    auto info_msg = raport.search("Serwer");
    for (const auto& msg : info_msg)
        std::cout << "  - " << msg << "\n";

    return 0;
}
