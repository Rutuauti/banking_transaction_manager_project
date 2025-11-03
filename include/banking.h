#ifndef BANKING_H
#define BANKING_H

#include "account.h"
#include "transaction.h"
#include "queue.h"
#include "stack.h"
#include <vector>
#include <string>


using TransactionQueue = Queue<Transaction>;
using TransactionStack = Stack<Transaction>;

class Banking {
private:
    std::vector<Account> accounts;       // List of all accounts
    TransactionQueue queue;              // Pending transactions
    TransactionStack doneStack;          // Completed transactions
    TransactionStack undoStack;          // For redo functionality
    int nextAccountNumber = 1001;        // Auto-incrementing account number

    
    Account* findAccount(int accNo);

public:
    
    Account* createAccount(const std::string &name, double balance, int age = 18);
    bool deleteAccount(int accNo);
    void displayAllAccounts() const;

    
    bool saveAccountsToFile(const std::string &filename);
    bool loadAccountsFromFile(const std::string &filename);

    
    bool deposit(int accNo, double amount);
    bool withdraw(int accNo, double amount);
    bool transfer(int fromAcc, int toAcc, double amount);

    
    bool enqueueTransaction(const Transaction &t);
    bool processNextTransaction(std::string &outMsg);
    void processAllTransactions();
    bool undoLast(std::string &outMsg);
    bool redoLast(std::string &outMsg);

    
    int getNextAccountNumber() const;

    
    bool canPerformTransaction(int accNo) const; 
};

#endif // BANKING_H
