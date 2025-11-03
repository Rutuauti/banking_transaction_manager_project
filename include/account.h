#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <iomanip>

struct Account {
    int accNo;               
    std::string name;        
    double balance;          
    int age;                 
    int transactionCount;    

    
    Account(int a = 0, const std::string &n = "", double b = 0.0, int ag = 18)
        : accNo(a), name(n), balance(b), age(ag), transactionCount(0) {}

    
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactionCount++;
        } else {
            std::cerr << "⚠️ Invalid deposit amount!\n";
        }
    }

    
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

    
    void display() const {
        std::cout << std::left 
                  << std::setw(20) << name
                  << std::setw(8)  << age
                  << "₹" << std::fixed << std::setprecision(2)
                  << balance
                  << "   Txns: " << transactionCount
                  << "\n";
    }

    
    int getAge() const { return age; }
    int getTransactionCount() const { return transactionCount; }

    
    void incrementTransactionCount() { transactionCount++; }
};

#endif // ACCOUNT_H
