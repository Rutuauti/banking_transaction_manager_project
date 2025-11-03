#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <vector>
#include <stdexcept>  // for std::out_of_range

// Generic Stack class template using STL vector
template <typename T>
class Stack {
private:
    std::vector<T> elements; // internal storage

public:
    Stack() = default; // ✅ Default constructor

    // Push element onto stack
    void push(const T& item) {
        elements.push_back(item);
    }

    // Pop element from stack and return it
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty!");
        }
        T item = elements.back();
        elements.pop_back();
        return item;
    }

    // View top element without removing
    T top() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty!");
        }
        return elements.back();
    }

    // Check if stack is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Clear all elements
    void clear() {
        elements.clear();
    }

    // Get number of elements in stack
    size_t size() const {
        return elements.size();
    }
};

#endif // STACK_H
