#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <array>
#include <algorithm>
#include <numeric>

// ============================================================
// Demonstracja: Kontenery sekwencyjne
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o sequence main.cpp && ./sequence
// ============================================================

void demo_vector() {
    std::cout << "=== std::vector ===\n";
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    std::cout << "size=" << v.size() << " capacity=" << v.capacity() << "\n";

    v.push_back(5);
    v.emplace_back(3);

    std::sort(v.begin(), v.end());

    std::cout << "Po sort: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // reserve – zapobiegaj realokacji
    std::vector<int> v2;
    v2.reserve(100);
    for (int i = 0; i < 100; ++i) v2.push_back(i);
    std::cout << "Po 100x push_back (z reserve): size=" << v2.size()
              << " capacity=" << v2.capacity() << "\n";

    // at() z kontrolą zakresu
    try {
        v.at(1000);
    } catch (const std::out_of_range& e) {
        std::cout << "Wyjątek: " << e.what() << "\n";
    }
}

void demo_deque() {
    std::cout << "\n=== std::deque ===\n";
    std::deque<int> d = {3, 4, 5};

    d.push_front(2);
    d.push_front(1);
    d.push_back(6);

    std::cout << "Deque: ";
    for (int x : d) std::cout << x << " ";  // 1 2 3 4 5 6
    std::cout << "\n";

    d.pop_front();
    d.pop_back();
    std::cout << "Po pop: ";
    for (int x : d) std::cout << x << " ";  // 2 3 4 5
    std::cout << "\n";
}

void demo_list() {
    std::cout << "\n=== std::list ===\n";
    std::list<int> lst = {1, 3, 5, 2, 4};

    // Wstawianie przed elementem
    auto it = std::find(lst.begin(), lst.end(), 3);
    lst.insert(it, 99);

    // Własny sort listy (std::sort nie działa – brak random access)
    lst.sort();
    lst.unique();  // usuń kolejne duplikaty

    std::cout << "List po sort+unique: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // splice – przenoszenie bez kopii
    std::list<int> lst2 = {100, 200};
    lst.splice(lst.begin(), lst2);
    std::cout << "Po splice: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "lst2 po splice: size=" << lst2.size() << "\n";  // 0
}

void demo_array() {
    std::cout << "\n=== std::array ===\n";
    std::array<int, 5> a = {5, 3, 1, 4, 2};

    std::sort(a.begin(), a.end());

    std::cout << "Posortowana: ";
    for (int x : a) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "size=" << a.size() << " (constexpr)\n";

    // Kompatybilność z C API przez data()
    int* ptr = a.data();
    std::cout << "Przez wskaźnik: " << ptr[0] << "\n";
}

void demo_porownanie() {
    std::cout << "\n=== Porównanie wydajności push_back ===\n";
    // Demonstracja mechanizmu podwajania
    std::vector<int> v;
    std::cout << "Kolejne capacity po push_back:\n";
    size_t prev_cap = 0;
    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
        if (v.capacity() != prev_cap) {
            std::cout << "  size=" << v.size()
                      << " capacity=" << v.capacity() << "\n";
            prev_cap = v.capacity();
        }
    }
}

int main() {
    demo_vector();
    demo_deque();
    demo_list();
    demo_array();
    demo_porownanie();
    return 0;
}
