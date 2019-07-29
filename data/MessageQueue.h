//
// Created by Andrew Chupin on 2019-02-13.
//

#ifndef SOCKET_MESSAGEQUEUE_H
#define SOCKET_MESSAGEQUEUE_H

#include <vector>
#include "Message.h"

class MessageQueue {

public:
    MessageQueue();
    ~MessageQueue();

    void put(Message *message);
    Message* pop();

    bool hasData();
    void clean();

private:
    std::vector<Message*> queue;
};

#endif //SOCKET_MESSAGEQUEUE_H
