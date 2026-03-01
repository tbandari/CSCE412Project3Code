/**
 * @file RequestQueue.cpp
 * @brief Implementation of the RequestQueue class providing a standard FIFO queue for web requests.
 */

#include "RequestQueue.h"
#include <stdexcept>

/**
 * @brief Pushes a new request to the back of the queue.
 * * @param r The Request object to enqueue.
 */
void RequestQueue::enqueue(const Request &r) {
    q.push(r);
}

/**
 * @brief Removes and returns the request at the front of the queue.
 * * @throw std::runtime_error if the queue is empty when called.
 * @return The Request object that was at the front of the queue.
 */
Request RequestQueue::dequeue() {
    if (q.empty()) {
        throw std::runtime_error("dequeue from empty queue");
    }
    Request r = q.front();
    q.pop();
    return r;
}

/**
 * @brief Checks whether the queue contains any requests.
 * * @return true if the queue is empty, false otherwise.
 */
bool RequestQueue::isEmpty() const {
    return q.empty();
}

/**
 * @brief Retrieves the total number of requests currently waiting in the queue.
 * * @return The count of elements in the queue.
 */
size_t RequestQueue::size() const {
    return q.size();
}
