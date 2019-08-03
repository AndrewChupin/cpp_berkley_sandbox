//
// Created by Andrew Chupin on 2019-02-13.
//

#ifndef SOCKET_MESSAGEQUEUE_H
#define SOCKET_MESSAGEQUEUE_H

#include <vector>
#include <deque>
#include <mutex>
#include "Message.h"

class MessageQueue {

public:
    MessageQueue();
    ~MessageQueue();

    void put(const std::shared_ptr<Message>& message);
    std::shared_ptr<Message> pop();
    std::shared_ptr<Message> next();

    bool hasData();
    void clear();

private:
    std::mutex mMutex;
    std::deque<std::shared_ptr<Message>> queue;

};

#endif //SOCKET_MESSAGEQUEUE_H
