//
// Created by Andrew Chupin on 2019-02-13.
//

#include "MessageQueue.h"


loop::MessageQueue::MessageQueue() = default;

loop::MessageQueue::~MessageQueue() {
    clear();
}

void loop::MessageQueue::put(const def::MessagePtr& message) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (message == nullptr) {
        return;
    }
    queue.push_back(message);
}

def::MessagePtr loop::MessageQueue::pop() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return nullptr;
    }

    auto message = queue.front();
    queue.pop_front();

    return message;
}

def::MessagePtr loop::MessageQueue::next() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return nullptr;
    }

    return queue.front();
}

bool loop::MessageQueue::hasData() {
    std::lock_guard<std::mutex> lock(mMutex);
    return !queue.empty();
}

void loop::MessageQueue::clear() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return;
    }

    queue.clear();
}
