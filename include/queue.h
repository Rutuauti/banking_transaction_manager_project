#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <queue>
#include <stdexcept>  


template <typename T>
class Queue {
private:
    std::queue<T> elements;

public:
    Queue() = default; 

    
    void enqueue(const T& item) {
        elements.push(item);
    }

    
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty!");
        }
        T frontItem = elements.front();
        elements.pop();
        return frontItem;
    }

    
    T front() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty!");
        }
        return elements.front();
    }

    
    bool isEmpty() const {
        return elements.empty();
    }

    
    void clear() {
        while (!elements.empty())
            elements.pop();
    }

    
    size_t size() const {
        return elements.size();
    }
};

#endif // QUEUE_H
