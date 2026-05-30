#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <type_traits>
#include <functional>

// ============================================================
// Demonstracja: Ewolucja generyczności – makra → void* → szablony
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o history main.cpp && ./history
// ============================================================

// ============================================================
// ETAP 1: Makra – niebezpieczna generyczność
// ============================================================
#define MACRO_MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MACRO_SWAP(T, x, y) do { T _tmp = (x); (x) = (y); (y) = _tmp; } while(0)

void demo_makra() {
    std::cout << "=== Makra (styl C) ===\n";

    int   a = 3,   b = 7;
    double c = 1.5, d = 2.5;

    std::cout << "MAX(3,7) = "    << MACRO_MAX(a, b) << "\n";
    std::cout << "MAX(1.5,2.5) = " << MACRO_MAX(c, d) << "\n";

    // Pułapka: efekty uboczne (skomentowane – nie wykonuj!)
    // int i = 3, j = 7;
    // int r = MACRO_MAX(i++, j++);  // i i j inkrementowane DWUKROTNIE
    // std::cout << "i=" << i << " j=" << j;  // 5 i 9, nie 4 i 8!

    std::cout << "(Pułapka z i++ jest bezpiecznie zakomentowana)\n";

    MACRO_SWAP(int, a, b);
    std::cout << "Po SWAP: a=" << a << " b=" << b << "\n";
}

// ============================================================
// ETAP 2: void* – "generyczność" bez bezpieczeństwa typów
// ============================================================
struct VoidStack {
    void** data;
    int size;
    int capacity;

    VoidStack() : data(nullptr), size(0), capacity(0) {}
    ~VoidStack() { delete[] data; }

    void push(void* item) {
        if (size == capacity) {
            int nc = capacity == 0 ? 4 : capacity * 2;
            void** nd = new void*[nc];
            for (int i = 0; i < size; ++i) nd[i] = data[i];
            delete[] data;
            data = nd;
            capacity = nc;
        }
        data[size++] = item;
    }

    void* pop() { return data[--size]; }
    bool empty() const { return size == 0; }
};

void demo_void_ptr() {
    std::cout << "\n=== void* (styl C++) ===\n";

    VoidStack s;
    int x = 10, y = 20, z = 30;
    s.push(&x); s.push(&y); s.push(&z);

    // Konieczne rzutowanie – brak bezpieczeństwa typów!
    std::cout << "pop: " << *(int*)s.pop() << "\n";
    std::cout << "pop: " << *(int*)s.pop() << "\n";

    // Można przypadkowo wrzucić double* i wyjąć jako int* → UB!
    double bad = 3.14;
    s.push(&bad);
    // int result = *(int*)s.pop();  // UB – silently wrong answer
    std::cout << "(Błąd typów jest bezpiecznie zakomentowany – byłby UB)\n";
}

// ============================================================
// ETAP 3: Szablony C++98 – bezpieczna generyczność
// ============================================================
template<typename T>
class Stos {
    std::vector<T> dane_;
public:
    void push(const T& x) { dane_.push_back(x); }
    void push(T&& x)      { dane_.push_back(std::move(x)); }
    T    pop()            {
        T x = std::move(dane_.back());
        dane_.pop_back();
        return x;
    }
    bool empty() const  { return dane_.empty(); }
    int  size()  const  { return (int)dane_.size(); }
};

template<typename T>
T tmpl_max(T a, T b) { return a > b ? a : b; }

void demo_szablony() {
    std::cout << "\n=== Szablony C++98 ===\n";

    // Trzy niezależne typy – kompilator generuje 3 klasy
    Stos<int>         si;
    Stos<std::string> ss;
    Stos<double>      sd;

    si.push(1); si.push(2); si.push(3);
    ss.push("alfa"); ss.push("beta");
    sd.push(1.1); sd.push(2.2);

    std::cout << "int stos pop: " << si.pop() << "\n";
    std::cout << "string stos pop: " << ss.pop() << "\n";
    std::cout << "double stos pop: " << sd.pop() << "\n";

    std::cout << "max(3,7): " << tmpl_max(3, 7) << "\n";
    std::cout << "max(1.5,2.5): " << tmpl_max(1.5, 2.5) << "\n";
    std::cout << "max(\"abc\",\"xyz\"): "
              << tmpl_max(std::string{"abc"}, std::string{"xyz"}) << "\n";

    // Kompilator ODMAWIA złego użycia (w przeciwieństwie do void*):
    // si.push("hello");  // BŁĄD KOMPILACJI – poprawne zachowanie
}

// ============================================================
// ETAP 4: SFINAE (C++11) – warunkowe szablony
// ============================================================
// Włącz tylko gdy T jest arytmetyczny
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
bezpieczna_suma(T a, T b) {
    return a + b;
}

// C++17 – czytelniejszy styl
template<typename T>
std::enable_if_t<std::is_integral_v<T>, bool>
jest_parzysta(T n) {
    return n % 2 == 0;
}

// if constexpr (C++17) – czytelniejsze niż SFINAE
template<typename T>
std::string opisz_typ(T val) {
    if constexpr (std::is_integral_v<T>)
        return "calkowity: " + std::to_string(val);
    else if constexpr (std::is_floating_point_v<T>)
        return "zmiennoprzecinkowy: " + std::to_string(val);
    else
        return "nieznany typ";
}

void demo_sfinae() {
    std::cout << "\n=== SFINAE + type_traits (C++11/17) ===\n";

    std::cout << "suma(3, 4) = " << bezpieczna_suma(3, 4) << "\n";
    std::cout << "suma(1.5, 2.5) = " << bezpieczna_suma(1.5, 2.5) << "\n";
    // bezpieczna_suma("abc", "def");  // BŁĄD kompilacji – std::string nie jest arytmetyczny

    std::cout << "jest_parzysta(4) = " << jest_parzysta(4) << "\n";
    std::cout << "jest_parzysta(7) = " << jest_parzysta(7) << "\n";

    std::cout << opisz_typ(42)    << "\n";
    std::cout << opisz_typ(3.14)  << "\n";
    std::cout << opisz_typ('A')   << "\n";
}

// ============================================================
// Porównanie: kod generyczny przed i po C++20
// ============================================================
void porownanie() {
    std::cout << "\n=== Porownanie styli ===\n";
    std::cout << "Makro:   brak typów, niebezpieczne efekty uboczne\n";
    std::cout << "void*:   jeden typ danych, niebezpieczne rzutowania\n";
    std::cout << "Szablon: bezpieczny, szybki, niejasne błędy\n";
    std::cout << "SFINAE:  warunki kompilacji, trudna składnia\n";
    std::cout << "Concepts (C++20): jasne kontrakty, czytelne błędy\n";
}

int main() {
    demo_makra();
    demo_void_ptr();
    demo_szablony();
    demo_sfinae();
    porownanie();
    return 0;
}
