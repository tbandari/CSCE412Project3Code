#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include "Request.h"
#include <queue>

/**
 * @file RequestQueue.h
 * @brief RequestQueue: FIFO queue wrapper for Request objects
 */

/**
 * @class RequestQueue
 * @brief A standard FIFO queue encapsulation holding incoming Request objects.
 */
class RequestQueue {
public:
    /**
     * @brief Default constructor.
     */
    RequestQueue() = default;

    /**
     * @brief Pushes a new request to the back of the queue.
     * @param r The Request object to enqueue.
     */
    void enqueue(const Request &r);

    /**
     * @brief Pops and returns the request at the front of the queue.
     * @warning The caller must ensure the queue is not empty before calling.
     * @return The Request object at the front of the queue.
     */
    Request dequeue();

    /**
     * @brief Checks whether the queue contains any requests.
     * @return true if the queue is empty, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Gets the total number of requests currently waiting in the queue.
     * @return The count of elements in the queue.
     */
    size_t size() const;

private:
    std::queue<Request> q; ///< Underlying standard library queue storing the requests
};

#endif