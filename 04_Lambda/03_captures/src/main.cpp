#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include <memory>

// ============================================================
// Demonstracja: Przechwycenie i domknięcia
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o captures main.cpp && ./captures
// ============================================================

// ============================================================
// 1. Porównanie trybów przechwycenia
// ============================================================
void demo_tryby() {
    std::cout << "=== Tryby przechwycenia ===\n";

    int a = 10, b = 20, c = 30;

    // Brak przechwycenia – tylko globalne/statyczne
    auto f0 = [](){ return 42; };  // OK – żadna lokalna nie używana
    std::cout << "f0: " << f0() << "\n";

    // Kopia konkretna
    auto f1 = [a](){ return a; };
    a = 100;
    std::cout << "f1 (kopia a=10 przed zmianą): " << f1() << "\n";  // 10

    // Referencja konkretna
    int suma = 0;
    auto f2 = [&suma](int x){ suma += x; };
    f2(5); f2(10); f2(15);
    std::cout << "suma (przez ref): " << suma << "\n";  // 30

    // Kopia wszystkiego [=]
    a = 10; b = 20; c = 30;
    auto f3 = [=](){ return a + b + c; };
    a = 999; b = 999; c = 999;
    std::cout << "f3 [=] (zamrozzone 10+20+30): " << f3() << "\n";  // 60

    // Referencja do wszystkiego [&]
    a = 1; b = 2; c = 3;
    auto f4 = [&](){ return a + b + c; };
    a = 10; b = 20; c = 30;
    std::cout << "f4 [&] (widzi zmiany 10+20+30): " << f4() << "\n";  // 60 (ale inne powody!)

    // Mieszane [=, &suma]
    a = 1;
    suma = 0;
    auto f5 = [=, &suma](int x){ suma += x + a; };  // a przez wartość, suma przez ref
    f5(10); f5(20);
    std::cout << "f5 [=,&suma], a=1: suma = " << suma << "\n";  // (10+1) + (20+1) = 32
}

// ============================================================
// 2. Pułapka – wisząca referencja
// ============================================================
std::function<int()> niebezpieczna_lambda() {
    int x = 42;
    // return [&x]() { return x; };  // NIEBEZPIECZNE – x umrze!
    return [x]() { return x; };  // BEZPIECZNE – kopia x
}

std::function<int()> bezpieczna_lambda() {
    int x = 42;
    return [x]() { return x; };
}

void demo_dangling() {
    std::cout << "\n=== Bezpieczenstwo referencji ===\n";

    auto f = bezpieczna_lambda();
    std::cout << "Bezpieczna (kopia): " << f() << "\n";  // 42

    // Pokazanie problemu (skomentowane – UB):
    // auto g = niebezpieczna_lambda();
    // std::cout << g();  // UB!

    std::cout << "(Wersja z [&x] jest zakomentowana – spowodowałaby UB)\n";
}

// ============================================================
// 3. Pułapka – lambdy w pętlach
// ============================================================
void demo_petla() {
    std::cout << "\n=== Lambdy w petlach ===\n";

    // BŁĘDNA wersja – wszystkie dzielą TĄ SAMĄ zmienną i
    std::vector<std::function<int()>> zle;
    for (int i = 0; i < 5; ++i) {
        // Komentarz: to jest błąd – i jest przez referencję do pętli
        // for (int i=...) – i żyje przez całą pętlę
        // Ale po pętli i = 5
        zle.push_back([i](){ return i; });  // NAPRAWIONE – kopia przez wartość
    }
    std::cout << "Poprawne (kopia i): ";
    for (auto& f : zle) std::cout << f() << " ";
    std::cout << "\n";

    // Inicjalizator przechwycenia (C++14) – jeszcze wyraźniej
    std::vector<std::function<int()>> dobre2;
    for (int i = 0; i < 5; ++i) {
        dobre2.push_back([val = i](){ return val * val; });
    }
    std::cout << "Kwadraty (init-capture): ";
    for (auto& f : dobre2) std::cout << f() << " ";
    std::cout << "\n";
}

// ============================================================
// 4. Przechwycenie this w klasach
// ============================================================
class Licznik {
    int wartosc_ = 0;
    std::string nazwa_;
public:
    explicit Licznik(std::string n) : nazwa_(std::move(n)) {}

    // [this] – przez wskaźnik
    auto zrob_inkrement() {
        return [this]() {
            return ++wartosc_;
        };
    }

    // [*this] (C++17) – kopia obiektu
    auto zrob_snapshot() const {
        return [*this]() {
            return nazwa_ + "=" + std::to_string(wartosc_);
        };
    }

    std::string info() const {
        return nazwa_ + ":" + std::to_string(wartosc_);
    }
};

void demo_this() {
    std::cout << "\n=== Przechwycenie this ===\n";

    Licznik c{"klik"};
    auto ink = c.zrob_inkrement();
    auto snap = c.zrob_snapshot();  // kopia stanu: wartosc_=0

    std::cout << "Przed: " << c.info() << "\n";
    ink(); ink(); ink();
    std::cout << "Po 3 inkrement: " << c.info() << "\n";
    std::cout << "Snapshot (zamrozony): " << snap() << "\n";  // "klik=0"
}

// ============================================================
// 5. Inicjalizatory przechwycenia i move semantics
// ============================================================
void demo_move_capture() {
    std::cout << "\n=== Move w przechwyceniu (C++14) ===\n";

    // Przechwycenie przez przeniesienie
    std::vector<int> duzy_vector(1000, 42);
    std::cout << "Przed move: size = " << duzy_vector.size() << "\n";

    auto f = [v = std::move(duzy_vector)](){
        return v.size();
    };

    std::cout << "Po move: size = " << duzy_vector.size() << "\n";  // 0
    std::cout << "Lambda ma: " << f() << " elementów\n";             // 1000

    // Shared_ptr w domknięciu
    auto sp = std::make_shared<std::string>("współdzielony zasób");
    std::cout << "Przed lambda: use_count = " << sp.use_count() << "\n";

    auto g = [sp](){  // kopia shared_ptr – zwiększa ref count
        return *sp;
    };
    std::cout << "Po lambda:    use_count = " << sp.use_count() << "\n";  // 2
    std::cout << "g() = " << g() << "\n";
}

int main() {
    demo_tryby();
    demo_dangling();
    demo_petla();
    demo_this();
    demo_move_capture();
    return 0;
}
