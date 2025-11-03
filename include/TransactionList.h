#ifndef TRANSACTIONLIST_H
#define TRANSACTIONLIST_H



#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <type_traits>


#ifdef HAS_TRANSACTION_HEADER
#include "transaction.h"
#endif

// ----------------- detection helpers -----------------
template <typename, typename = void>
struct has_member_type_string : std::false_type {};

template <typename T>
struct has_member_type_string<T, typename std::enable_if<
    std::is_same<decltype(std::declval<T>().type), std::string>::value
>::type> : std::true_type {};

template <typename, typename = void>
struct has_member_type_enum : std::false_type {};

template <typename T>
struct has_member_type_enum<T, typename std::enable_if<
    !std::is_same<decltype(std::declval<T>().type), std::string>::value
>::type> : std::true_type {};


#define DECL_HAS_MEMBER(name) \
template <typename, typename = void> \
struct has_member_##name : std::false_type {}; \
template <typename T> \
struct has_member_##name<T, typename std::enable_if< \
    !std::is_same<decltype(std::declval<T>().name), void>::value \
>::type> : std::true_type {};

 

template <typename, typename = void>
struct has_accNo : std::false_type {};
template <typename T>
struct has_accNo<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().accNo), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_account : std::false_type {};
template <typename T>
struct has_account<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().account), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_fromAcc : std::false_type {};
template <typename T>
struct has_fromAcc<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().fromAcc), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_from_account : std::false_type {};
template <typename T>
struct has_from_account<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().from_account), void>::value, void>::type> : std::true_type {};

// target / to variants
template <typename, typename = void>
struct has_targetAcc : std::false_type {};
template <typename T>
struct has_targetAcc<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().targetAcc), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_toAcc : std::false_type {};
template <typename T>
struct has_toAcc<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().toAcc), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_to_account : std::false_type {};
template <typename T>
struct has_to_account<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().to_account), void>::value, void>::type> : std::true_type {};


template <typename, typename = void>
struct has_amount : std::false_type {};
template <typename T>
struct has_amount<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().amount), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_amt : std::false_type {};
template <typename T>
struct has_amt<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().amt), void>::value, void>::type> : std::true_type {};


template <typename, typename = void>
struct has_date : std::false_type {};
template <typename T>
struct has_date<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().date), void>::value, void>::type> : std::true_type {};

template <typename, typename = void>
struct has_balanceAfter : std::false_type {};
template <typename T>
struct has_balanceAfter<T, typename std::enable_if<!std::is_same<decltype(std::declval<T>().balanceAfter), void>::value, void>::type> : std::true_type {};


template <typename T>
std::string get_type_as_string(const T& tr) {
    if constexpr (std::is_same<decltype(tr.type), std::string>::value) {
        return tr.type;
    } else {
        using FnType = std::string(*)(decltype(tr.type));
        #if __cplusplus >= 201103L
        template <typename U>
        struct has_typeToStr {
            template <typename V>
            static auto test(int) -> decltype(typeToStr(std::declval<V>()), std::true_type());
            template <typename>
            static std::false_type test(...);
            static constexpr bool value = decltype(test<U>(0))::value;
        };
        #else
        struct has_typeToStr { static const bool value = false; };
        #endif

        
        #if __cplusplus >= 201103L
        if constexpr (has_typeToStr<decltype(tr.type)>::value) {
            return typeToStr(tr.type);
        } else {
            std::ostringstream oss;
            oss << tr.type;
            return oss.str();
        }
        #else
        std::ostringstream oss;
        oss << tr.type;
        return oss.str();
        #endif
    }
}

// acc number
template <typename T>
int get_accNo(const T& tr) {
    if constexpr (has_accNo<T>::value) return static_cast<int>(tr.accNo);
    else if constexpr (has_account<T>::value) return static_cast<int>(tr.account);
    else if constexpr (has_fromAcc<T>::value) return static_cast<int>(tr.fromAcc);
    else if constexpr (has_from_account<T>::value) return static_cast<int>(tr.from_account);
    else return 0; // unknown / unavailable
}

// target account
template <typename T>
int get_targetAcc(const T& tr) {
    if constexpr (has_targetAcc<T>::value) return static_cast<int>(tr.targetAcc);
    else if constexpr (has_toAcc<T>::value) return static_cast<int>(tr.toAcc);
    else if constexpr (has_to_account<T>::value) return static_cast<int>(tr.to_account);
    else return 0; // unknown/not-applicable
}

// amount
template <typename T>
double get_amount(const T& tr) {
    if constexpr (has_amount<T>::value) return static_cast<double>(tr.amount);
    else if constexpr (has_amt<T>::value) return static_cast<double>(tr.amt);
    else return 0.0;
}

// date 
template <typename T>
std::string get_date(const T& tr) {
    if constexpr (has_date<T>::value) return tr.date;
    else return std::string("");
}

// balanceAfter 
template <typename T>
double get_balanceAfter(const T& tr) {
    if constexpr (has_balanceAfter<T>::value) return static_cast<double>(tr.balanceAfter);
    else return 0.0;
}

// ----------------- TransactionList -----------------
class TransactionList {
public:
    template <typename Tx>
    static void displayMiniStatement(const std::vector<Tx>& transactions, size_t N = 5) {
        if (transactions.empty()) {
            std::cout << "No transactions found.\n";
            return;
        }

        bool hasDate = has_date<Tx>::value;
        bool hasBalAfter = has_balanceAfter<Tx>::value;

        // header
        if (hasDate) {
            std::cout << std::left << std::setw(20) << "Date/Time";
        }
        std::cout << std::left << std::setw(12) << "Type"
                  << std::setw(8) << "AccNo"
                  << std::setw(8) << "Target"
                  << std::setw(12) << "Amount";
        if (hasBalAfter) std::cout << std::setw(12) << "Balance";
        std::cout << '\n';

        size_t width = 20 + 12 + 8 + 8 + 12 + (hasBalAfter ? 12 : 0);
        if (!hasDate) width -= 20;
        std::cout << std::string((width>0?width:50), '-') << '\n';

        size_t count = 0;
        for (auto it = transactions.rbegin(); it != transactions.rend() && count < N; ++it, ++count) {
            if (hasDate) {
                std::cout << std::left << std::setw(20) << get_date(*it);
            }
            std::cout << std::left << std::setw(12) << get_type_as_string(*it)
                      << std::setw(8) << get_accNo(*it)
                      << std::setw(8) << get_targetAcc(*it)
                      << std::setw(12) << std::fixed << std::setprecision(2) << get_amount(*it);
            if (hasBalAfter) std::cout << std::setw(12) << std::fixed << std::setprecision(2) << get_balanceAfter(*it);
            std::cout << '\n';
        }
    }
};

#endif // TRANSACTIONLIST_H
