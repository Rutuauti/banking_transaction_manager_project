#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <iomanip>

// 🏦 Represents a single bank account node
struct Account {
    int accNo;               // Unique account number
    std::string name;        // Account holder name
    double balance;          // Current balance amount
    int age;                 // 👶 Age of account holder
    int transactionCount;    // 🔢 Number of transactions done

    // Constructor
    Account(int a = 0, const std::string &n = "", double b = 0.0, int ag = 18)
        : accNo(a), name(n), balance(b), age(ag), transactionCount(0) {}

    // 💰 Deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactionCount++;
        } else {
            std::cerr << "⚠️ Invalid deposit amount!\n";
        }
    }

    // 💸 Withdraw money (with safety check)
    bool withdraw(double amount) {
        if (amount <= 0) {
            std::cerr << "⚠️ Invalid withdrawal amount!\n";
            return false;
        }
        if (amount > balance) {
            std::cerr << "❌ Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        transactionCount++;
        return true;
    }

    // 📊 Display account info neatly
    void display() const {
        std::cout << std::left << std::setw(10) << accNo
                  << std::setw(20) << name
                  << std::setw(8)  << age
                  << "₹" << std::fixed << std::setprecision(2)
                  << balance
                  << "   Txns: " << transactionCount
                  << "\n";
    }

    // ⚙️ Getters
    int getAge() const { return age; }
    int getTransactionCount() const { return transactionCount; }

    // 🧮 Increment transaction count (if needed manually)
    void incrementTransactionCount() { transactionCount++; }
};

#endif // ACCOUNT_H
