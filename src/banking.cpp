#include "banking.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <ctime>



static std::unordered_map<int, int> accountAges;


static std::unordered_map<int, std::vector<std::time_t>> accountTransactions;


static size_t cleanupOldTransactions(int accNo) {
    auto &times = accountTransactions[accNo];
    std::time_t now = std::time(nullptr);
    const std::time_t ONE_DAY = 24 * 60 * 60;

    std::vector<std::time_t> recent;
    for (auto t : times) {
        if (now - t < ONE_DAY)
            recent.push_back(t);
    }
    times.swap(recent);
    return times.size();
}


static bool canRecordTransaction(int accNo) {
    int age = 18; // default
    if (accountAges.count(accNo)) age = accountAges[accNo];

    if (age < 18) {
        size_t count = cleanupOldTransactions(accNo);
        if (count >= 20) {
            std::cerr << "⚠️ Transaction limit reached for minor account " << accNo << " (20 per day).\n";
            return false;
        }
        accountTransactions[accNo].push_back(std::time(nullptr));
        return true;
    }

    
    accountTransactions[accNo].push_back(std::time(nullptr));
    return true;
}


void setAccountAge(int accNo, int age) {
    accountAges[accNo] = age;
}


bool Banking::deposit(int accNo, double amount) {
    if (amount <= 0) return false;
    Account* a = findAccount(accNo);
    if (!a) return false;

    if (!canRecordTransaction(accNo)) return false; // 👈 minor limit check

    a->balance += amount;
    return true;
}


bool Banking::withdraw(int accNo, double amount) {
    if (amount <= 0) return false;
    Account* a = findAccount(accNo);
    if (!a || a->balance < amount) return false;

    if (!canRecordTransaction(accNo)) return false; // 👈 minor limit check

    a->balance -= amount;
    return true;
}


bool Banking::transfer(int fromAcc, int toAcc, double amount) {
    if (amount <= 0) return false;
    Account* from = findAccount(fromAcc);
    Account* to = findAccount(toAcc);
    if (!from || !to || from->balance < amount) return false;

    if (!canRecordTransaction(fromAcc)) return false; // 👈 minor limit check

    from->balance -= amount;
    to->balance += amount;
    return true;
}


bool Banking::enqueueTransaction(const Transaction& t) {
    queue.enqueue(t);
    return true;
}


bool Banking::processNextTransaction(std::string& outMsg) {
    if (queue.isEmpty()) {
        outMsg = "No pending transactions.";
        return false;
    }

    Transaction t = queue.dequeue();
    bool success = false;
    std::stringstream msg;

    switch (t.type) {
        case DEPOSIT:
            success = deposit(t.accNo, t.amount);
            msg << (success ? "Deposited " : "Failed deposit of ")
                << t.amount << " to Acc " << t.accNo;
            break;

        case WITHDRAW:
            success = withdraw(t.accNo, t.amount);
            msg << (success ? "Withdrew " : "Failed withdrawal of ")
                << t.amount << " from Acc " << t.accNo;
            break;

        case TRANSFER:
            success = transfer(t.accNo, t.targetAcc, t.amount);
            msg << (success ? "Transferred " : "Failed transfer of ")
                << t.amount << " from Acc " << t.accNo
                << " to Acc " << t.targetAcc;
            break;

        default:
            msg << "Unknown transaction type.";
            break;
    }

    outMsg = msg.str();
    if (success) doneStack.push(t);

    return success;
}


void Banking::processAllTransactions() {
    std::string msg;
    while (!queue.isEmpty()) {
        if (processNextTransaction(msg))
            std::cout << "✅ " << msg << "\n";
        else
            std::cout << "❌ " << msg << "\n";
    }
}


bool Banking::undoLast(std::string& outMsg) {
    if (doneStack.isEmpty()) {
        outMsg = "No transaction to undo.";
        return false;
    }

    Transaction t = doneStack.pop();
    bool ok = false;
    std::stringstream msg;

    switch (t.type) {
        case DEPOSIT:
            ok = withdraw(t.accNo, t.amount);
            msg << (ok ? "Undid deposit of " : "Failed to undo deposit of ")
                << t.amount << " from Acc " << t.accNo;
            break;

        case WITHDRAW:
            ok = deposit(t.accNo, t.amount);
            msg << (ok ? "Undid withdrawal of " : "Failed to undo withdrawal of ")
                << t.amount << " to Acc " << t.accNo;
            break;

        case TRANSFER:
            ok = transfer(t.targetAcc, t.accNo, t.amount);
            msg << (ok ? "Undid transfer of " : "Failed to undo transfer of ")
                << t.amount << " from Acc " << t.targetAcc
                << " to Acc " << t.accNo;
            break;

        default:
            msg << "Unknown transaction type.";
            break;
    }

    outMsg = msg.str();
    if (ok) undoStack.push(t);
    else doneStack.push(t);

    return ok;
}


bool Banking::redoLast(std::string& outMsg) {
    if (undoStack.isEmpty()) {
        outMsg = "No transaction to redo.";
        return false;
    }

    Transaction t = undoStack.pop();
    bool ok = false;
    std::stringstream msg;

    switch (t.type) {
        case DEPOSIT:
            ok = deposit(t.accNo, t.amount);
            msg << (ok ? "Redid deposit of " : "Failed to redo deposit of ")
                << t.amount << " to Acc " << t.accNo;
            break;

        case WITHDRAW:
            ok = withdraw(t.accNo, t.amount);
            msg << (ok ? "Redid withdrawal of " : "Failed to redo withdrawal of ")
                << t.amount << " from Acc " << t.accNo;
            break;

        case TRANSFER:
            ok = transfer(t.accNo, t.targetAcc, t.amount);
            msg << (ok ? "Redid transfer of " : "Failed to redo transfer of ")
                << t.amount << " from Acc " << t.accNo
                << " to Acc " << t.targetAcc;
            break;

        default:
            msg << "Unknown transaction type.";
            break;
    }

    outMsg = msg.str();
    if (ok) doneStack.push(t);
    else undoStack.push(t);

    return ok;
}


int Banking::getNextAccountNumber() const {
    return nextAccountNumber;
}
