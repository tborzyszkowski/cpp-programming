#pragma once
#include <iostream>
#include <string>
#include <vector>

// =============================================================
// Klasa BankAccount ilustruje składowe statyczne
// =============================================================
class BankAccount {
private:
    // --- Pola składowe (niestatyczne) – każdy obiekt ma swoją kopię ---
    std::string owner_;
    double      balance_;
    int         accountId_;      // numer przydzielony przy tworzeniu

    // --- Pola STATYCZNE – wspólne dla WSZYSTKICH obiektów klasy ---
    static int    nextId_;          // licznik kont
    static double interestRate_;    // stopa procentowa (wspólna dla wszystkich)
    static int    totalAccounts_;   // liczba żyjących obiektów

public:
    // Konstruktor
    BankAccount(const std::string& owner, double initialBalance = 0.0)
        : owner_(owner), balance_(initialBalance) {
        accountId_ = nextId_++;    // przydziel unikalny id
        totalAccounts_++;
        std::cout << "[BankAccount] Otwarto konto #" << accountId_
                  << " dla " << owner_ << "\n";
    }

    // Destruktor
    ~BankAccount() {
        totalAccounts_--;
        std::cout << "[BankAccount] Zamknięto konto #" << accountId_
                  << " (" << owner_ << ")\n";
    }

    // Operacje
    void deposit(double amount)  { balance_ += amount; }
    void withdraw(double amount) {
        if (amount > balance_) throw std::runtime_error("Brak środków");
        balance_ -= amount;
    }

    // Nalicz odsetki – używa static interestRate_
    void applyInterest() {
        balance_ *= (1.0 + interestRate_);
    }

    // Gettery
    std::string owner()     const { return owner_;     }
    double      balance()   const { return balance_;   }
    int         accountId() const { return accountId_; }

    std::string toString() const {
        return "BankAccount[#" + std::to_string(accountId_) +
               " " + owner_ + ", balance=" + std::to_string(balance_) + "]";
    }

    // --- Metody STATYCZNE – dostęp bez obiektu ---
    // Nie mają wskaźnika this, mogą używać TYLKO pól/metod statycznych
    static int    getNextId()        { return nextId_;        }
    static double getInterestRate()  { return interestRate_;  }
    static int    getTotalAccounts() { return totalAccounts_; }

    static void setInterestRate(double rate) {
        interestRate_ = rate;
        std::cout << "[static] Stopa procentowa zmieniona na: " << rate << "\n";
    }

    static void printStats() {
        std::cout << "[static] Aktywne konta: " << totalAccounts_
                  << ", następne ID: "           << nextId_
                  << ", stopa: "                 << interestRate_ << "\n";
    }
};

// --- Definicje składowych statycznych ---
// Muszą być zdefiniowane POZA klasą (w pliku .cpp lub inline w nagłówku)
inline int    BankAccount::nextId_         = 1000;
inline double BankAccount::interestRate_   = 0.05;   // 5%
inline int    BankAccount::totalAccounts_  = 0;
