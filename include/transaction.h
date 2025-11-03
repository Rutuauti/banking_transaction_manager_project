#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <queue>
#include <vector>

// 🏷️ Enum for the type of transaction
enum TransactionType { DEPOSIT, WITHDRAW, TRANSFER, UNKNOWN };

// 🔁 Converts string to TransactionType
inline TransactionType strToType(const std::string &s) {
    if (s == "DEPOSIT") return DEPOSIT;
    if (s == "WITHDRAW") return WITHDRAW;
    if (s == "TRANSFER") return TRANSFER;
    return UNKNOWN;
}

// 🔁 Converts TransactionType to string
inline std::string typeToStr(TransactionType t) {
    switch (t) {
        case DEPOSIT: return "DEPOSIT";
        case WITHDRAW: return "WITHDRAW";
        case TRANSFER: return "TRANSFER";
        default: return "UNKNOWN";
    }
}

// 🧾 Struct representing a single transaction
struct Transaction {
    TransactionType type;
    int accNo;         // Source account
    int targetAcc;     // Target account (for transfer)
    double amount;     // Amount involved
    std::time_t timestamp; // 🕒 Timestamp for tracking

    Transaction(TransactionType t = UNKNOWN, int a = 0, int b = 0, double amt = 0.0)
        : type(t), accNo(a), targetAcc(b), amount(amt) {
        timestamp = std::time(nullptr); // Record current time
    }
};

// 🧩 Class to handle **daily transaction limit tracking**
class DailyTransactionTracker {
private:
    std::queue<std::time_t> transactionTimes;  // Stores timestamps of recent transactions
    int maxTransactions;                       // Dynamic limit (varies with age)
    const int SECONDS_IN_DAY = 24 * 60 * 60;   // Seconds in a day

public:
    // Constructor: allows setting custom limit (default = 5)
    DailyTransactionTracker(int limit = 5) : maxTransactions(limit) {}

    // 🎯 Set limit dynamically (e.g., based on age)
    void setLimit(int limit) {
        maxTransactions = limit;
    }

    // ✅ Try to record a transaction
    // Returns true if allowed, false if limit reached
    bool recordTransaction() {
        std::time_t now = std::time(nullptr);

        // Remove outdated transactions (older than 1 day)
        while (!transactionTimes.empty() && (now - transactionTimes.front()) > SECONDS_IN_DAY) {
            transactionTimes.pop();
        }

        // Check limit
        if ((int)transactionTimes.size() >= maxTransactions) {
            return false; // ❌ Limit exceeded
        }

        // Record the new transaction
        transactionTimes.push(now);
        return true;
    }

    // 🧮 Get remaining transactions for the day
    int remainingTransactions() const {
        return maxTransactions - static_cast<int>(transactionTimes.size());
    }

    // 🔄 Reset tracker manually (e.g., at midnight)
    void reset() {
        while (!transactionTimes.empty()) transactionTimes.pop();
    }
};

#endif // TRANSACTION_H
