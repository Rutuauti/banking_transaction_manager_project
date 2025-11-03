#include "account.h"
#include <iostream>
#include <iomanip>


void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "✅ Deposited ₹" << amount << " successfully.\n";
    } else {
        std::cerr << "⚠️ Invalid deposit amount!\n";
    }
}


bool Account::withdraw(double amount) {
    if (amount <= 0) {
        std::cerr << "⚠️ Invalid withdrawal amount!\n";
        return false;
    }
    if (amount > balance) {
        std::cerr << "❌ Insufficient balance!\n";
        return false;
    }
    balance -= amount;
    std::cout << "💸 Withdrawn ₹" << amount << " successfully.\n";
    return true;
}


void Account::display() const {
    std::cout << std::left << std::setw(10) << accNo
              << std::setw(20) << name
              << "₹" << std::fixed << std::setprecision(2)
              << balance << "\n";
}
