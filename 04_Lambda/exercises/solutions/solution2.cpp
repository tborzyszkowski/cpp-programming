#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

// ============================================================
// Rozwiązanie – Ćwiczenie 2: Scheduler
// ============================================================
// Kompilacja:
//   g++ -std=c++17 -o solution2 solution2.cpp && ./solution2
// ============================================================

class Scheduler {
    struct Task {
        int delay;
        std::function<void()> action;
    };
    std::vector<Task> queue_;

public:
    void schedule(int delay, std::function<void()> task) {
        queue_.push_back({delay, std::move(task)});
    }

    void tick() {
        // Zbierz zadania gotowe do wykonania (delay == 0)
        // Zmniejsz delay pozostałych o 1
        std::vector<std::function<void()>> do_run;

        for (auto& t : queue_) {
            if (t.delay == 0)
                do_run.push_back(std::move(t.action));
            else
                --t.delay;
        }

        // Usuń wykonane (delay był 0 → action przeniesiony)
        queue_.erase(
            std::remove_if(queue_.begin(), queue_.end(),
                           [](const Task& t){ return !t.action; }),
            queue_.end()
        );

        // Wykonaj zadania (w kolejności dodania – zachowanej przez vector)
        for (auto& action : do_run) action();
    }

    int pending_count() const {
        return static_cast<int>(queue_.size());
    }

    void cancel_all() {
        queue_.clear();
    }
};

// ============================================================
// Testy
// ============================================================
void test_podstawowy() {
    std::cout << "=== Test podstawowy ===\n";

    Scheduler s;
    std::vector<std::string> log;

    s.schedule(0, [&log]{ log.push_back("natychmiast"); });
    s.schedule(2, [&log]{ log.push_back("po 2 tickach"); });
    s.schedule(1, [&log]{ log.push_back("po 1 ticku"); });
    s.schedule(2, [&log]{ log.push_back("po 2 tickach (drugie)"); });

    std::cout << "Oczekujace: " << s.pending_count() << "\n";  // 4

    s.tick();  // wykonuje: "natychmiast"
    std::cout << "Po tick 1, log: ";
    for (const auto& e : log) std::cout << "\"" << e << "\" ";
    std::cout << "\n";
    std::cout << "Oczekujace: " << s.pending_count() << "\n";  // 3

    s.tick();  // wykonuje: "po 1 ticku"
    std::cout << "Po tick 2, log: ";
    for (const auto& e : log) std::cout << "\"" << e << "\" ";
    std::cout << "\n";

    s.tick();  // wykonuje: "po 2 tickach", "po 2 tickach (drugie)"
    std::cout << "Po tick 3, log: ";
    for (const auto& e : log) std::cout << "\"" << e << "\" ";
    std::cout << "\n";
    std::cout << "Oczekujace: " << s.pending_count() << "\n";  // 0
}

void test_cancel() {
    std::cout << "\n=== Test cancel_all ===\n";

    Scheduler s;
    int wykonane = 0;

    s.schedule(1, [&wykonane]{ ++wykonane; });
    s.schedule(2, [&wykonane]{ ++wykonane; });
    s.schedule(3, [&wykonane]{ ++wykonane; });

    std::cout << "Przed cancel: " << s.pending_count() << "\n";  // 3
    s.cancel_all();
    std::cout << "Po cancel:    " << s.pending_count() << "\n";  // 0

    s.tick(); s.tick(); s.tick();
    std::cout << "Wykonane (powinno byc 0): " << wykonane << "\n";
}

void test_lambdy_z_przechwyceniem() {
    std::cout << "\n=== Lambdy z przechwyceniem ===\n";

    Scheduler s;
    int licznik = 0;
    std::string status = "start";

    // Lambdy przechwytują zmienne przez referencję
    s.schedule(0, [&licznik, &status]{
        ++licznik;
        status = "tick0";
    });
    s.schedule(1, [&licznik, &status]{
        licznik += 10;
        status = "tick1";
    });

    s.tick();
    std::cout << "licznik=" << licznik << ", status=" << status << "\n";  // 1, tick0

    s.tick();
    std::cout << "licznik=" << licznik << ", status=" << status << "\n";  // 11, tick1
}

void test_kolejnosc() {
    std::cout << "\n=== Test kolejnosci (FIFO dla tego samego delay) ===\n";

    Scheduler s;
    std::vector<int> kolejnosc;

    // 3 zadania z tym samym delay = 0
    s.schedule(0, [&kolejnosc]{ kolejnosc.push_back(1); });
    s.schedule(0, [&kolejnosc]{ kolejnosc.push_back(2); });
    s.schedule(0, [&kolejnosc]{ kolejnosc.push_back(3); });

    s.tick();
    std::cout << "Kolejnosc wykonania: ";
    for (int x : kolejnosc) std::cout << x << " ";
    std::cout << "(oczekiwano: 1 2 3)\n";
}

int main() {
    test_podstawowy();
    test_cancel();
    test_lambdy_z_przechwyceniem();
    test_kolejnosc();
    std::cout << "\nWszystkie testy zakonczone.\n";
    return 0;
}
