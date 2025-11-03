#ifndef BANKING_H
#define BANKING_H

#include "account.h"
#include "transaction.h"
#include "queue.h"
#include "stack.h"
#include <vector>
#include <string>

// Type aliases for clarity
using TransactionQueue = Queue<Transaction>;
using TransactionStack = Stack<Transaction>;

class Banking {
private:
    std::vector<Account> accounts;       // List of all accounts
    TransactionQueue queue;              // Pending transactions
    TransactionStack doneStack;          // Completed transactions
    TransactionStack undoStack;          // For redo functionality
    int nextAccountNumber = 1001;        // Auto-incrementing account number

    // Helper to find account by number
    Account* findAccount(int accNo);

public:
    // ---- CRUD Operations ----
    Account* createAccount(const std::string &name, double balance, int age = 18);
    bool deleteAccount(int accNo);
    void displayAllAccounts() const;

    // ---- File Handling ----
    bool saveAccountsToFile(const std::string &filename);
    bool loadAccountsFromFile(const std::string &filename);

    // ---- Core Transactions ----
    bool deposit(int accNo, double amount);
    bool withdraw(int accNo, double amount);
    bool transfer(int fromAcc, int toAcc, double amount);

    // ---- Queue + Stack Operations ----
    bool enqueueTransaction(const Transaction &t);
    bool processNextTransaction(std::string &outMsg);
    void processAllTransactions();
    bool undoLast(std::string &outMsg);
    bool redoLast(std::string &outMsg);

    // ---- Utility ----
    int getNextAccountNumber() const;

    // ---- Age-based Rule ----
    bool canPerformTransaction(int accNo) const; // Checks if under-18 reached 20 txns
};

#endif // BANKING_H
