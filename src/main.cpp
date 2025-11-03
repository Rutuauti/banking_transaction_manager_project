#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>
#include "stack.h"
#include "queue.h"
#include "TransactionList.h"

using namespace std;


struct Transaction {
    string type;
    double amount;
    double balanceAfter;
    string date;
};


string currentDateTime() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buf;
}


void saveTransactionsToFile(const string& username, const vector<Transaction>& transactions) {
    ofstream file(username + "_transactions.txt", ios::trunc);
    for (const auto& txn : transactions) {
        file << txn.date << "," << txn.type << "," << txn.amount << "," << txn.balanceAfter << "\n";
    }
    file.close();
}


vector<Transaction> loadTransactionsFromFile(const string& username) {
    vector<Transaction> transactions;
    ifstream file(username + "_transactions.txt");
    string date, type;
    double amount, balance;

    while (getline(file, date, ',')) {
        getline(file, type, ',');
        file >> amount;
        file.ignore();
        file >> balance;
        file.ignore();
        transactions.push_back({type, amount, balance, date});
    }

    file.close();
    return transactions;
}


double getBalance(const vector<Transaction>& transactions) {
    if (transactions.empty()) return 0.0;
    return transactions.back().balanceAfter;
}


void printMiniStatement(const vector<Transaction>& transactions) {
    cout << left << setw(20) << "Date" << setw(15) << "Type"
         << setw(15) << "Amount" << setw(15) << "Balance" << "\n";
    cout << string(65, '-') << "\n";
    int count = 0;
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < 5; ++it, ++count) {
        cout << left << setw(20) << it->date
             << setw(15) << it->type
             << setw(15) << fixed << setprecision(2) << it->amount
             << setw(15) << it->balanceAfter << "\n";
    }
}

int main(int argc, char* argv[]) {
    
    if (argc > 1) {
        string command = argv[1];

        if (command == "deposit" && argc == 4) {
            string username = argv[2];
            double amount = stod(argv[3]);
            auto transactions = loadTransactionsFromFile(username);
            double balance = getBalance(transactions) + amount;

            transactions.push_back({"Deposit", amount, balance, currentDateTime()});
            saveTransactionsToFile(username, transactions);

            cout << "Deposited " << amount << " to " << username
                 << ". New balance: " << balance << endl;
            return 0;
        }

        else if (command == "withdraw" && argc == 4) {
            string username = argv[2];
            double amount = stod(argv[3]);
            auto transactions = loadTransactionsFromFile(username);
            double balance = getBalance(transactions);

            if (amount > balance) {
                cerr << "Insufficient funds." << endl;
                return 1;
            }

            balance -= amount;
            transactions.push_back({"Withdraw", amount, balance, currentDateTime()});
            saveTransactionsToFile(username, transactions);

            cout << "Withdrew " << amount << " from " << username
                 << ". Remaining balance: " << balance << endl;
            return 0;
        }

        else if (command == "transfer" && argc == 5) {
            string fromUser = argv[2], toUser = argv[3];
            double amount = stod(argv[4]);
            auto fromTxns = loadTransactionsFromFile(fromUser);
            auto toTxns = loadTransactionsFromFile(toUser);
            double fromBal = getBalance(fromTxns);
            double toBal = getBalance(toTxns);

            if (amount > fromBal) {
                cerr << "Insufficient funds in " << fromUser << endl;
                return 1;
            }

            fromBal -= amount;
            toBal += amount;

            fromTxns.push_back({"TransferOut->" + toUser, amount, fromBal, currentDateTime()});
            toTxns.push_back({"TransferIn<-" + fromUser, amount, toBal, currentDateTime()});

            saveTransactionsToFile(fromUser, fromTxns);
            saveTransactionsToFile(toUser, toTxns);

            cout << "Transferred " << amount << " from " << fromUser << " to " << toUser << endl;
            return 0;
        }

        else if (command == "mini-statement" && argc == 3) {
            string username = argv[2];
            auto txns = loadTransactionsFromFile(username);
            printMiniStatement(txns);
            return 0;
        }

        else {
            cerr << "Invalid command or arguments." << endl;
            return 1;
        }
    }

    
    string username;
    cout << "Enter username: ";
    cin >> username;

    auto transactions = loadTransactionsFromFile(username);
    double balance = getBalance(transactions);
    Stack<Transaction> undoStack;
    Stack<Transaction> redoStack;

    int choice;
    do {
        cout << "\n1. Deposit\n2. Withdraw\n3. Show Balance\n4. Mini Statement"
             << "\n5. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            double amount;
            cout << "Enter amount: ";
            cin >> amount;
            balance += amount;
            transactions.push_back({"Deposit", amount, balance, currentDateTime()});
            cout << "Deposited " << amount << ". New balance: " << balance << endl;
        } else if (choice == 2) {
            double amount;
            cout << "Enter amount: ";
            cin >> amount;
            if (amount > balance) {
                cout << "Insufficient funds.\n";
            } else {
                balance -= amount;
                transactions.push_back({"Withdraw", amount, balance, currentDateTime()});
                cout << "Withdrawn " << amount << ". Remaining: " << balance << endl;
            }
        } else if (choice == 3) {
            cout << "Current Balance: ₹" << balance << endl;
        } else if (choice == 4) {
            printMiniStatement(transactions);
        }
    } while (choice != 5);

    saveTransactionsToFile(username, transactions);
    cout << "Data saved. Exiting...\n";
    return 0;
}
