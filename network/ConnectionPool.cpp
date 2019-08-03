//
// Created by Andrew Chupin on 2019-02-03.
//

#include <thread>
#include "ConnectionPool.h"
#include "../event/MessagePool.h"
#include "../common/Util.h"
#include "../common/Logger.h"

net::ConnectionPool::~ConnectionPool() {
    for (const auto& socket : mSockets) {
        socket->destroy();
    }

    mQueue.clear();
    mSockets.clear();
}

void net::ConnectionPool::create() {
    /*const auto& socket = std::make_shared<net::NetSocket>();
    socket->create();
    mSockets.push_back(socket);*/
    auto message = MessagePool::getInstance()->obtain();
    message->time = util::now() + 2000;
    mQueue.put(message);
}

void net::ConnectionPool::loop() {
    int64_t now;
    int64_t nextTimeout;
    std::shared_ptr<Message> nextMessage;

    for (;;) {
        LOG_D("loop\n");
        if (isFinished) {
            return;
        }

        nextMessage = mQueue.next();
        now = util::now();
        nextTimeout = def::DEFAULT_EVENT_TIMEOUT;

        if (nextMessage != nullptr) {
            if (now >= nextMessage->time) {
                auto message = mQueue.pop();

                LOG_D("EVENT\n");
                // TODO EVENT

                MessagePool::getInstance()->recycle(message);
                nextMessage = mQueue.next();

                if (nextMessage != nullptr) {
                    nextTimeout = 0;
                }
            } else {
                nextTimeout = nextMessage->time - now;
            }
        }

        if (nextTimeout == 0) {
            continue;
        }

        if (nextTimeout > def::DEFAULT_EVENT_TIMEOUT) {
            nextTimeout = def::DEFAULT_EVENT_TIMEOUT;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(nextTimeout));
    }
}
