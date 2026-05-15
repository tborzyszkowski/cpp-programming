#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <numeric>

template<typename T>
class DataSet {
    std::vector<T> dane_;
public:
    void add(const T& v) { dane_.push_back(v); }

    std::optional<T> min() const {
        if (dane_.empty()) return std::nullopt;
        return *std::min_element(dane_.begin(), dane_.end());
    }

    std::optional<T> max() const {
        if (dane_.empty()) return std::nullopt;
        return *std::max_element(dane_.begin(), dane_.end());
    }

    std::optional<double> mean() const {
        if (dane_.empty()) return std::nullopt;
        double suma = std::accumulate(dane_.begin(), dane_.end(), 0.0);
        return suma / static_cast<double>(dane_.size());
    }

    std::optional<double> median() const {
        if (dane_.empty()) return std::nullopt;
        auto kopia = sorted();
        std::size_t n = kopia.size();
        if (n % 2 == 1)
            return static_cast<double>(kopia[n / 2]);
        return (static_cast<double>(kopia[n/2 - 1]) +
                static_cast<double>(kopia[n/2])) / 2.0;
    }

    std::vector<T> sorted() const {
        auto kopia = dane_;
        std::sort(kopia.begin(), kopia.end());
        return kopia;
    }

    template<typename Pred>
    std::vector<T> filter(Pred pred) const {
        std::vector<T> wynik;
        std::copy_if(dane_.begin(), dane_.end(),
                     std::back_inserter(wynik), pred);
        return wynik;
    }
};

int main() {
    DataSet<int> ds;
    for (int x : {5, 3, 8, 1, 9, 2, 7})
        ds.add(x);

    std::cout << "min:    " << *ds.min()    << "\n";  // 1
    std::cout << "max:    " << *ds.max()    << "\n";  // 9
    std::cout << "mean:   " << *ds.mean()   << "\n";  // ~5
    std::cout << "median: " << *ds.median() << "\n";  // 5

    auto parzyste = ds.filter([](int x){ return x % 2 == 0; });
    std::cout << "parzyste:";
    for (int x : parzyste) std::cout << " " << x;
    std::cout << "\n";

    auto posortowane = ds.sorted();
    std::cout << "posortowane:";
    for (int x : posortowane) std::cout << " " << x;
    std::cout << "\n";

    // Test pustego zbioru
    DataSet<double> pusty;
    std::cout << "pusty min: " << (pusty.min() ? "wartość" : "nullopt") << "\n";

    return 0;
}
