#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <algorithm>

// ============================================================
// Demonstracja: std::string, string_view, stringstream
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o string_demo main.cpp && ./string_demo
// ============================================================

void demo_string_podstawy() {
    std::cout << "=== std::string – podstawy ===\n";
    std::string s = "Ala ma kota";

    std::cout << "size=" << s.size() << " capacity=" << s.capacity() << "\n";
    std::cout << "front='" << s.front() << "' back='" << s.back() << "'\n";

    // Wyszukiwanie
    auto pos = s.find("ma");
    std::cout << "\"ma\" na pozycji: " << pos << "\n";  // 4
    std::cout << "\"psa\": " << (s.find("psa") == std::string::npos
        ? "nie znaleziono" : "znaleziono") << "\n";

    // Wycinanie
    std::cout << "substr(4,2): " << s.substr(4, 2) << "\n";  // "ma"

    // Modyfikacja
    std::string kopia = s;
    kopia.replace(4, 2, "nie ma");
    std::cout << "replace: " << kopia << "\n";

    // Składanie
    std::string wynik;
    for (int i = 1; i <= 5; ++i)
        wynik += std::to_string(i) + " ";
    std::cout << "Złożony: " << wynik << "\n";
}

void demo_konwersje() {
    std::cout << "\n=== Konwersje ===\n";

    // string → liczba
    std::string s1 = "42";
    std::string s2 = "3.14";
    int i = std::stoi(s1);
    double d = std::stod(s2);
    std::cout << "stoi: " << i << " stod: " << d << "\n";

    // liczba → string
    std::string si = std::to_string(1234567);
    std::string sd = std::to_string(2.71828);
    std::cout << "to_string int: " << si << "\n";
    std::cout << "to_string double: " << sd << "\n";

    // Obsługa błędów
    try {
        std::stoi("nie_liczba");
    } catch (const std::invalid_argument& e) {
        std::cout << "invalid_argument: " << e.what() << "\n";
    }
}

// Funkcja przyjmująca string_view – zero kopii dla string, C-string i literałów
size_t policz_samogloski(std::string_view sv) {
    const std::string_view samogloski = "aeiouAEIOU";
    return std::count_if(sv.begin(), sv.end(), [&samogloski](char c){
        return samogloski.find(c) != std::string_view::npos;
    });
}

void demo_string_view() {
    std::cout << "\n=== std::string_view ===\n";

    std::string s = "Hello World";
    const char* cstr = "Hello C-string";

    std::cout << "\"Hello World\" samogloski: " << policz_samogloski(s)    << "\n";
    std::cout << "C-string samogloski: "        << policz_samogloski(cstr) << "\n";
    std::cout << "Literał samogloski: "         << policz_samogloski("Ala ma kota") << "\n";

    // Widok na fragment – zero kopii
    std::string_view sv = s;
    std::string_view fragment = sv.substr(6, 5);  // widok na "World"
    std::cout << "Fragment: " << fragment << "\n";
    std::cout << "starts_with Hello: " << std::boolalpha << sv.starts_with("Hello") << "\n";
}

void demo_stringstream() {
    std::cout << "\n=== std::stringstream ===\n";

    // Budowanie
    std::ostringstream oss;
    oss << "Wyniki: ";
    std::vector<int> v = {3, 1, 4, 1, 5};
    for (int x : v) oss << x << " ";
    std::cout << oss.str() << "\n";

    // Parsowanie
    std::string rekord = "Anna 30 165.5";
    std::istringstream iss(rekord);
    std::string imie;
    int wiek;
    double wzrost;
    iss >> imie >> wiek >> wzrost;
    std::cout << imie << ", wiek=" << wiek << ", wzrost=" << wzrost << "\n";

    // Tokenizacja CSV
    std::string csv = "Jan,Feb,Mar,Apr,May";
    std::istringstream csvss(csv);
    std::string token;
    std::cout << "Miesiące: ";
    while (std::getline(csvss, token, ','))
        std::cout << "[" << token << "] ";
    std::cout << "\n";
}

void demo_cpp20() {
    std::cout << "\n=== C++20 ===\n";
    std::string s = "Hello, World!";

    std::cout << "starts_with \"Hello\": " << std::boolalpha << s.starts_with("Hello") << "\n";
    std::cout << "ends_with \"World!\": "  << s.ends_with("World!") << "\n";
}

int main() {
    demo_string_podstawy();
    demo_konwersje();
    demo_string_view();
    demo_stringstream();
    demo_cpp20();
    return 0;
}
