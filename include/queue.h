#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <queue>
#include <stdexcept>  // for std::out_of_range

// Generic Queue class template using STL queue
template <typename T>
class Queue {
private:
    std::queue<T> elements;

public:
    Queue() = default; // ✅ Default constructor

    // Add element to the queue
    void enqueue(const T& item) {
        elements.push(item);
    }

    // Remove and return front element
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty!");
        }
        T frontItem = elements.front();
        elements.pop();
        return frontItem;
    }

    // Peek front element without removing it
    T front() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty!");
        }
        return elements.front();
    }

    // Check if queue is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Remove all elements
    void clear() {
        while (!elements.empty())
            elements.pop();
    }

    // Get number of elements in the queue
    size_t size() const {
        return elements.size();
    }
};

#endif // QUEUE_H
