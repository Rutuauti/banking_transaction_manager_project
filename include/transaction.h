#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <queue>
#include <vector>


enum TransactionType { DEPOSIT, WITHDRAW, TRANSFER, UNKNOWN };


inline TransactionType strToType(const std::string &s) {
    if (s == "DEPOSIT") return DEPOSIT;
    if (s == "WITHDRAW") return WITHDRAW;
    if (s == "TRANSFER") return TRANSFER;
    return UNKNOWN;
}


inline std::string typeToStr(TransactionType t) {
    switch (t) {
        case DEPOSIT: return "DEPOSIT";
        case WITHDRAW: return "WITHDRAW";
        case TRANSFER: return "TRANSFER";
        default: return "UNKNOWN";
    }
}


struct Transaction {
    TransactionType type;
    int accNo;         
    int targetAcc;     
    double amount;     
    std::time_t timestamp; 

    Transaction(TransactionType t = UNKNOWN, int a = 0, int b = 0, double amt = 0.0)
        : type(t), accNo(a), targetAcc(b), amount(amt) {
        timestamp = std::time(nullptr); 
    }
};


class DailyTransactionTracker {
private:
    std::queue<std::time_t> transactionTimes;  
    int maxTransactions;                       
    const int SECONDS_IN_DAY = 24 * 60 * 60;   

public:
    
    DailyTransactionTracker(int limit = 20) : maxTransactions(limit) {}

    
    void setLimit(int limit) {
        maxTransactions = limit;
    }

    
    bool recordTransaction() {
        std::time_t now = std::time(nullptr);

        
        while (!transactionTimes.empty() && (now - transactionTimes.front()) > SECONDS_IN_DAY) {
            transactionTimes.pop();
        }

        
        if ((int)transactionTimes.size() >= maxTransactions) {
            return false; // ❌ Limit exceeded
        }

        
        transactionTimes.push(now);
        return true;
    }

    
    int remainingTransactions() const {
        return maxTransactions - static_cast<int>(transactionTimes.size());
    }

    
    void reset() {
        while (!transactionTimes.empty()) transactionTimes.pop();
    }
};

#endif // TRANSACTION_H
