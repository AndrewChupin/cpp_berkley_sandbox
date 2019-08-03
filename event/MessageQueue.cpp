//
// Created by Andrew Chupin on 2019-02-13.
//

#include "MessageQueue.h"


MessageQueue::MessageQueue() = default;

MessageQueue::~MessageQueue() {
    clear();
}

void MessageQueue::put(const std::shared_ptr<Message>& message) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (message == nullptr) {
        return;
    }
    queue.push_back(message);
}

std::shared_ptr<Message> MessageQueue::pop() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return nullptr;
    }

    auto message = queue.front();
    queue.pop_front();

    return message;
}

bool MessageQueue::hasData() {
    std::lock_guard<std::mutex> lock(mMutex);
    return !queue.empty();
}

void MessageQueue::clear() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return;
    }

    queue.clear();
}

std::shared_ptr<Message> MessageQueue::next() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (queue.empty()) {
        return nullptr;
    }

    return queue.front();
}
