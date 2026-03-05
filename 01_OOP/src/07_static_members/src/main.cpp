#include <iostream>
#include "BankAccount.h"

// ============================================================
// Demonstracja: składowe statyczne klasy BankAccount
// Kompilacja: g++ -std=c++17 -o static_members src/main.cpp
// ============================================================

int main() {
    std::cout << "=== Składowe statyczne ===\n";

    // Dostęp do metody statycznej BEZ tworzenia obiektu
    std::cout << "Przed stworzeniem kont:\n";
    BankAccount::printStats();

    std::cout << "\n--- Tworzenie kont ---\n";
    BankAccount alice("Alice", 1000.0);
    BankAccount bob("Bob", 500.0);
    BankAccount charlie("Charlie");

    BankAccount::printStats();

    std::cout << "\n--- Ustawianie stopy procentowej (statycznie) ---\n";
    BankAccount::setInterestRate(0.03);   // 3%, zmienia dla WSZYSTKICH
    BankAccount::printStats();

    std::cout << "\n--- Naliczenie odsetek na wszystkich kontach ---\n";
    alice.applyInterest();
    bob.applyInterest();
    charlie.applyInterest();
    std::cout << alice.toString()   << "\n";
    std::cout << bob.toString()     << "\n";
    std::cout << charlie.toString() << "\n";

    std::cout << "\n--- Operacje na kontach ---\n";
    alice.deposit(200.0);
    bob.withdraw(100.0);
    std::cout << alice.toString() << "\n";
    std::cout << bob.toString()   << "\n";

    std::cout << "\n--- Dostęp do statycznych przez obiekt (możliwe, ale nie zalecane) ---\n";
    std::cout << "alice.getTotalAccounts() = "
              << alice.getTotalAccounts() << "\n";   // to samo co ::getTotalAccounts()
    std::cout << "BankAccount::getTotalAccounts() = "
              << BankAccount::getTotalAccounts() << "\n";

    std::cout << "\n--- Koniec scope – destruktory ---\n";
    {
        BankAccount temp("Temp", 100.0);
        BankAccount::printStats();
    }  // tu destruktor temp: totalAccounts_ zmniejszone
    BankAccount::printStats();

    std::cout << "\n=== Koniec main ===\n";
    return 0;
}
