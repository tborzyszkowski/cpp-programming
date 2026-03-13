#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>

// Forward declaration
class Auditor;

// =============================================================
// Klasa BankAccount
// =============================================================
class BankAccount {
private:
    std::string          owner_;
    double               balance_;
    std::vector<double>  transactions_;   // + wpłata, - wypłata; 0.0 = nieudana próba
    int                  failedWithdrawals_;

public:
    BankAccount(const std::string& owner, double initialBalance)
        : owner_(owner), balance_(initialBalance), failedWithdrawals_(0)
    {
        transactions_.push_back(initialBalance);   // otwarcie konta
    }

    void deposit(double amount) {
        if (amount <= 0.0) return;
        balance_ += amount;
        transactions_.push_back(+amount);
    }

    // Zwraca true jeśli wypłata się powiodła
    bool withdraw(double amount) {
        if (amount <= 0.0) return false;
        if (amount > balance_) {
            ++failedWithdrawals_;
            transactions_.push_back(-0.0);   // NaN-proxy: nieudana próba (oznaczamy 0)
            return false;
        }
        balance_ -= amount;
        transactions_.push_back(-amount);
        return true;
    }

    // ---- zaprzyjaźnienia ----
    friend class Auditor;
    friend std::ostream& operator<<(std::ostream& os, const BankAccount& acc);
};

// =============================================================
// operator<< jako friend – wypisuje podstawowe info
// =============================================================
inline std::ostream& operator<<(std::ostream& os, const BankAccount& acc) {
    os << "Konto [" << acc.owner_ << "]"
       << "  saldo: " << std::fixed << std::setprecision(2) << acc.balance_ << " PLN";
    return os;
}

// =============================================================
// Klasa Auditor – friend BankAccount, może czytać prywatne pola
// =============================================================
class Auditor {
private:
    std::string auditorName_;

public:
    explicit Auditor(const std::string& name) : auditorName_(name) {}

    // Średnia wartość UDANYCH transakcji (pomija zerowe = nieudane próby)
    double averageTransaction(const BankAccount& acc) const {
        std::vector<double> valid;
        for (double t : acc.transactions_)
            if (t != 0.0) valid.push_back(t);
        if (valid.empty()) return 0.0;
        return std::accumulate(valid.begin(), valid.end(), 0.0)
               / static_cast<double>(valid.size());
    }

    bool hasOverdraftAttempt(const BankAccount& acc) const {
        return acc.failedWithdrawals_ > 0;   // dostęp do prywatnego pola!
    }

    void generateReport(const BankAccount& acc) const {
        const auto& tx = acc.transactions_;

        // Min/max wśród niezerowych transakcji
        std::vector<double> valid;
        for (double t : tx) if (t != 0.0) valid.push_back(t);

        double minTx = valid.empty() ? 0.0 : *std::min_element(valid.begin(), valid.end());
        double maxTx = valid.empty() ? 0.0 : *std::max_element(valid.begin(), valid.end());
        double avg   = averageTransaction(acc);

        std::cout << "\n=== Raport Audytora: " << acc.owner_ << " ==="
                  << "  [audytor: " << auditorName_ << "]\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Saldo końcowe    : " << acc.balance_ << " PLN\n";
        std::cout << "Liczba operacji  : " << tx.size() << "\n";
        std::cout << "Historia         : ";
        for (double t : tx) {
            if (t > 0.0)      std::cout << "+" << t << "  ";
            else if (t < 0.0) std::cout << t << "  ";
            else              std::cout << "[ODRZUCONA]  ";
        }
        std::cout << "\n";
        std::cout << "Min: " << minTx << "  Max: " << maxTx << "  Avg: " << avg << "\n";
        std::cout << "Próby bez pokrycia: " << acc.failedWithdrawals_ << "\n";
        std::cout << std::string(50, '=') << "\n";
    }
};
