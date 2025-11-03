#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <vector>
#include <stdexcept>  // for std::out_of_range


template <typename T>
class Stack {
private:
    std::vector<T> elements; 

public:
    Stack() = default; 

    
    void push(const T& item) {
        elements.push_back(item);
    }

    
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty!");
        }
        T item = elements.back();
        elements.pop_back();
        return item;
    }

    
    T top() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty!");
        }
        return elements.back();
    }

    
    bool isEmpty() const {
        return elements.empty();
    }

    
    void clear() {
        elements.clear();
    }

    
    size_t size() const {
        return elements.size();
    }
};

#endif // STACK_H
