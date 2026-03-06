#include <iostream>
#include "BankAccount.h"

// ============================================================
// Rozwiązanie Zadania 1: BankAccount + Auditor (friend class)
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex1_bank main.cpp
// ============================================================

int main() {
    BankAccount alice("Anna Nowak",  1000.0);
    BankAccount bob  ("Piotr Wiśniewski", 500.0);

    // operator<< (friend function)
    std::cout << alice << "\n";
    std::cout << bob   << "\n";

    // Operacje na koncie Anny
    alice.deposit(500.0);
    alice.withdraw(300.0);
    alice.withdraw(200.0);
    alice.deposit(450.0);
    alice.withdraw(5000.0);   // próba bez pokrycia!

    // Operacje na koncie Piotra
    bob.deposit(200.0);
    bob.withdraw(150.0);
    bob.withdraw(1000.0);     // próba bez pokrycia!
    bob.withdraw(1000.0);     // próba bez pokrycia!

    // Audytor weryfikuje oba konta
    Auditor auditor("Jan Kontroler");
    auditor.generateReport(alice);
    auditor.generateReport(bob);

    // Pomocnicze metody
    std::cout << "\n--- Sprawdzenie ---\n";
    std::cout << "Avg transakcja (Anna) : "
              << auditor.averageTransaction(alice) << " PLN\n";
    std::cout << "Nadmierne wypłaty (Piotr): "
              << std::boolalpha << auditor.hasOverdraftAttempt(bob) << "\n";

    return 0;
}
