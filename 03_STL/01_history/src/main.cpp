#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>

// ============================================================
// Demonstracja: Przed STL vs. z STL
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o history main.cpp && ./history
// ============================================================

// --- Przed STL: ręczna tablica dynamiczna -------------------
struct RecznaLista {
    int*  data;
    int   size;
    int   capacity;

    RecznaLista() : data(nullptr), size(0), capacity(0) {}
    ~RecznaLista() { delete[] data; }

    void push(int val) {
        if (size == capacity) {
            int newCap = capacity == 0 ? 1 : capacity * 2;
            int* newData = new int[newCap];
            for (int i = 0; i < size; ++i) newData[i] = data[i];
            delete[] data;
            data = newData;
            capacity = newCap;
        }
        data[size++] = val;
    }

    // Ręczne sortowanie bąbelkowe
    void sortuj() {
        for (int i = 0; i < size - 1; ++i)
            for (int j = 0; j < size - i - 1; ++j)
                if (data[j] > data[j + 1])
                    std::swap(data[j], data[j + 1]);
    }
};

// --- Z STL: vector + algorytmy ------------------------------
void demo_stl() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3};

    std::sort(v.begin(), v.end());

    int suma = std::accumulate(v.begin(), v.end(), 0);

    auto it = std::find(v.begin(), v.end(), 8);

    std::cout << "vector po sort: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\nSuma: " << suma;
    std::cout << "\nPozycja 8: " << std::distance(v.begin(), it) << "\n";
}

// --- Trzy filary: kontener + iterator + algorytm ------------
void demo_filary() {
    std::cout << "\n=== Trzy filary STL ===\n";

    // Ten sam algorytm działa na różnych kontenerach
    std::vector<int> vec  = {3, 1, 4, 1, 5, 9, 2, 6};
    std::list<int>   lst  = {3, 1, 4, 1, 5, 9, 2, 6};

    int suma_vec = std::accumulate(vec.begin(), vec.end(), 0);
    int suma_lst = std::accumulate(lst.begin(), lst.end(), 0);

    std::cout << "Suma (vector): " << suma_vec << "\n";
    std::cout << "Suma (list):   " << suma_lst << "\n";
    // Identyczny wynik – algorytm nie wie o typie kontenera
}

// --- Mapa: kontener asocjacyjny -----------------------------
void demo_map() {
    std::cout << "\n=== std::map ===\n";

    std::map<std::string, int> oceny;
    oceny["Anna"]   = 5;
    oceny["Bartek"] = 4;
    oceny["Celina"] = 5;

    // Iteracja w kolejności alfabetycznej (map jest posortowana)
    for (const auto& [imie, ocena] : oceny)
        std::cout << imie << ": " << ocena << "\n";
}

int main() {
    std::cout << "=== Przed STL (ręczna lista) ===\n";
    RecznaLista rl;
    for (int x : {5, 2, 8, 1, 9, 3}) rl.push(x);
    rl.sortuj();
    std::cout << "Po sortowaniu: ";
    for (int i = 0; i < rl.size; ++i) std::cout << rl.data[i] << " ";
    std::cout << "\n";

    std::cout << "\n=== Z STL ===\n";
    demo_stl();
    demo_filary();
    demo_map();

    return 0;
}
