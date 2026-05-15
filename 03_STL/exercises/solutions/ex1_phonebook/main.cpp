#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <algorithm>
#include <ostream>

class PhoneBook {
    std::unordered_map<std::string, std::string> kontakty_;
public:
    void add(const std::string& name, const std::string& number) {
        kontakty_[name] = number;
    }

    bool remove(const std::string& name) {
        return kontakty_.erase(name) > 0;
    }

    std::optional<std::string> find(const std::string& name) const {
        auto it = kontakty_.find(name);
        if (it == kontakty_.end()) return std::nullopt;
        return it->second;
    }

    int count_by_prefix(const std::string& prefix) const {
        return static_cast<int>(
            std::count_if(kontakty_.begin(), kontakty_.end(),
                [&prefix](const auto& kv){
                    return kv.first.substr(0, prefix.size()) == prefix;
                }));
    }

    std::vector<std::pair<std::string, std::string>> list_sorted() const {
        std::vector<std::pair<std::string, std::string>> v(
            kontakty_.begin(), kontakty_.end());
        std::sort(v.begin(), v.end());
        return v;
    }

    friend std::ostream& operator<<(std::ostream& os, const PhoneBook& pb) {
        for (const auto& [name, number] : pb.list_sorted())
            os << name << ": " << number << "\n";
        return os;
    }
};

int main() {
    PhoneBook pb;
    pb.add("Anna",   "111-222-333");
    pb.add("Bartek", "444-555-666");
    pb.add("Agata",  "777-888-999");
    pb.add("Celina", "000-111-222");

    std::cout << "=== Wszystkie kontakty ===\n" << pb;

    auto nr = pb.find("Anna");
    if (nr) std::cout << "\nAnna: " << *nr << "\n";

    auto brak = pb.find("Zofia");
    std::cout << "Zofia: " << (brak ? *brak : "brak") << "\n";

    std::cout << "\nKontakty na 'A': " << pb.count_by_prefix("A") << "\n";

    pb.remove("Bartek");
    std::cout << "\n=== Po usunięciu Bartka ===\n" << pb;

    return 0;
}
