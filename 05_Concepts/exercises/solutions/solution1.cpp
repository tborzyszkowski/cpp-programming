#include <iostream>
#include <concepts>
#include <array>
#include <stdexcept>
#include <string>
#include <algorithm>

// ============================================================
// Rozwiązanie 1: Stack<T> z Concepts
// ============================================================
// Kompilacja:
//   g++ -std=c++20 -o solution1 solution1.cpp && ./solution1
// ============================================================

// ---- Wyjątki ----
struct StackOverflow : std::runtime_error {
    explicit StackOverflow(std::size_t cap)
        : std::runtime_error("Stack pełny (pojemność=" + std::to_string(cap) + ")") {}
};

struct StackUnderflow : std::runtime_error {
    StackUnderflow() : std::runtime_error("Stack pusty") {}
};

// ---- Concept ----
template<typename T>
concept Stosowalny = std::copyable<T> && std::equality_comparable<T>;

// ---- Stack<T, N> ----
template<Stosowalny T, std::size_t N = 64>
class Stack {
    std::array<T, N> dane_;
    std::size_t      top_ = 0;

public:
    // Push – kopia
    void push(const T& x) {
        if (top_ == N) throw StackOverflow{N};
        dane_[top_++] = x;
    }

    // Push – move
    void push(T&& x) {
        if (top_ == N) throw StackOverflow{N};
        dane_[top_++] = std::move(x);
    }

    // Pop
    T pop() {
        if (top_ == 0) throw StackUnderflow{};
        return std::move(dane_[--top_]);
    }

    // Top
    const T& top() const {
        if (top_ == 0) throw StackUnderflow{};
        return dane_[top_ - 1];
    }

    bool        empty()    const { return top_ == 0; }
    bool        full()     const { return top_ == N; }
    std::size_t size()     const { return top_; }
    std::size_t capacity() const { return N; }

    void clear() { top_ = 0; }

    // contains() korzysta z equality_comparable (jest w Stosowalny)
    bool contains(const T& x) const {
        for (std::size_t i = 0; i < top_; ++i)
            if (dane_[i] == x) return true;
        return false;
    }
};

// ---- Testy ----
int main() {
    std::cout << "=== Stack<int, 4> ===\n";

    Stack<int, 4> s;
    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << std::boolalpha;
    std::cout << "size=" << s.size() << " full=" << s.full() << "\n";
    std::cout << "top=" << s.top() << "\n";
    std::cout << "contains(20)=" << s.contains(20) << "\n";
    std::cout << "contains(99)=" << s.contains(99) << "\n";

    s.push(40);
    std::cout << "Po 4 push: full=" << s.full() << "\n";

    // Przepełnienie
    try {
        s.push(50);
    } catch (const StackOverflow& e) {
        std::cout << "Wyjątek: " << e.what() << "\n";
    }

    // Pop
    std::cout << "pop=" << s.pop() << "\n";
    std::cout << "pop=" << s.pop() << "\n";
    std::cout << "size=" << s.size() << "\n";

    // Underflow
    s.clear();
    try {
        s.pop();
    } catch (const StackUnderflow& e) {
        std::cout << "Wyjątek: " << e.what() << "\n";
    }

    std::cout << "\n=== Stack<std::string, 3> ===\n";
    Stack<std::string, 3> ss;
    ss.push("alfa");
    ss.push(std::string{"beta"});
    ss.push("gamma");
    std::cout << "top=" << ss.top() << "\n";
    std::cout << "contains(\"beta\")=" << ss.contains("beta") << "\n";
    ss.pop();
    ss.pop();
    std::cout << "Po 2 pop: size=" << ss.size() << " top=" << ss.top() << "\n";

    // Concept odrzuca unique_ptr (nie copyable):
    // Stack<std::unique_ptr<int>> ups;   // BŁĄD KOMPILACJI
    std::cout << "\n(Stack<std::unique_ptr<int>> nie skompiluje się – zakomentowane)\n";

    return 0;
}
