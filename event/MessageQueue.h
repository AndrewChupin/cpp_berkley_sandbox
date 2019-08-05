//
// Created by Andrew Chupin on 2019-02-13.
//

#ifndef SOCKET_MESSAGEQUEUE_H
#define SOCKET_MESSAGEQUEUE_H

#include <vector>
#include <deque>
#include <mutex>
#include "Message.h"
#include "../common/Logger.h"

namespace loop {

    class MessageQueue {

    public:
        MessageQueue();
        ~MessageQueue();

        void put(const def::MessagePtr& message);
        def::MessagePtr pop();
        def::MessagePtr next();

        bool hasData();
        void clear();

    private:
        std::mutex mMutex;
        std::deque<std::shared_ptr<Message>> queue;

    };

}

#endif //SOCKET_MESSAGEQUEUE_H
