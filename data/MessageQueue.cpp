//
// Created by Andrew Chupin on 2019-02-13.
//

#include "MessageQueue.h"


MessageQueue::MessageQueue() = default;

MessageQueue::~MessageQueue() {
    clean();
}

void MessageQueue::put(Message *message) {
    if (message == nullptr) {
        return;
    }
    queue.push_back(message);
}

Message* MessageQueue::pop() {
    if (queue.empty()) {
        return nullptr;
    }

    return queue[queue.size() - 1];
}

bool MessageQueue::hasData() {
    return !queue.empty();
}

void MessageQueue::clean() {
    if (queue.empty()) {
        return;
    }

    size_t size = queue.size();
    for (uint32_t a = 0; a < size; a++) {
        delete queue[a];
    }

    queue.clear();
}
