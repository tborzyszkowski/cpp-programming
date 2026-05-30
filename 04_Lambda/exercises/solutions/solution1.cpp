#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include <cmath>

// ============================================================
// Rozwiązanie – Ćwiczenie 1: FunctionPipeline<T>
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o solution1 solution1.cpp && ./solution1
// ============================================================

template<typename T>
class FunctionPipeline {
    using Step = std::function<T(T)>;
    std::vector<Step> steps_;

public:
    FunctionPipeline& pipe(Step f) {
        steps_.push_back(std::move(f));
        return *this;
    }

    FunctionPipeline& pipe_if(std::function<bool(const T&)> pred, Step f) {
        steps_.push_back([pred = std::move(pred), f = std::move(f)](T x) -> T {
            return pred(x) ? f(std::move(x)) : std::move(x);
        });
        return *this;
    }

    T run(T value) const {
        for (const auto& step : steps_)
            value = step(std::move(value));
        return value;
    }

    void reset() {
        steps_.clear();
    }

    int size() const { return static_cast<int>(steps_.size()); }
};

// ============================================================
// Testy
// ============================================================
void test_int_pipeline() {
    std::cout << "=== FunctionPipeline<int> ===\n";

    FunctionPipeline<int> p;
    int mnoznik = 3;

    p.pipe([](int x){ return x * 2; })
     .pipe([mnoznik](int x){ return x + mnoznik; })
     .pipe_if([](int x){ return x > 10; }, [](int x){ return x - 5; })
     .pipe([](int x){ return x * x; });

    // run(3): 3*2=6, +3=9, 9>10? NIE, 9*9=81
    std::cout << "run(3)  = " << p.run(3)  << " (oczekiwano 81)\n";

    // run(10): 10*2=20, +3=23, 23>10? TAK → 23-5=18, 18*18=324
    std::cout << "run(10) = " << p.run(10) << " (oczekiwano 324)\n";

    // run(0): 0*2=0, +3=3, 3>10? NIE, 3*3=9
    std::cout << "run(0)  = " << p.run(0)  << " (oczekiwano 9)\n";
}

void test_string_pipeline() {
    std::cout << "\n=== FunctionPipeline<string> ===\n";

    FunctionPipeline<std::string> sp;
    sp.pipe([](std::string s){
          std::transform(s.begin(), s.end(), s.begin(), ::toupper);
          return s;
      })
      .pipe([](std::string s){ return "[" + s + "]"; })
      .pipe_if([](const std::string& s){ return s.size() > 5; },
               [](std::string s){ return s + "!"; });

    std::cout << "run(\"hello\")   = " << sp.run("hello")   << "\n";  // "[HELLO]!"
    std::cout << "run(\"hi\")      = " << sp.run("hi")      << "\n";  // "[HI]" (nie > 5 po bracketach)
    std::cout << "run(\"world\")   = " << sp.run("world")   << "\n";  // "[WORLD]!"
}

void test_double_pipeline() {
    std::cout << "\n=== FunctionPipeline<double> (analiza danych) ===\n";

    FunctionPipeline<double> dp;
    double threshold = 100.0;

    dp.pipe([](double x){ return x * 1.1; })                             // +10%
      .pipe_if([threshold](double x){ return x > threshold; },
               [](double x){ return x * 0.9; })                          // rabat gdy > 100
      .pipe([](double x){ return std::round(x * 100.0) / 100.0; });      // zaokrągl

    for (double cena : {50.0, 100.0, 200.0}) {
        std::cout << "cena=" << cena << " → " << dp.run(cena) << "\n";
    }
}

void test_reset() {
    std::cout << "\n=== Reset ===\n";

    FunctionPipeline<int> p;
    p.pipe([](int x){ return x + 1; })
     .pipe([](int x){ return x * 100; });

    std::cout << "run(5) przed reset: " << p.run(5) << "\n";  // (5+1)*100 = 600

    p.reset();
    p.pipe([](int x){ return x - 1; });
    std::cout << "run(5) po reset:    " << p.run(5) << "\n";  // 5-1 = 4
}

int main() {
    test_int_pipeline();
    test_string_pipeline();
    test_double_pipeline();
    test_reset();
    std::cout << "\nWszystkie testy zakonczone.\n";
    return 0;
}
