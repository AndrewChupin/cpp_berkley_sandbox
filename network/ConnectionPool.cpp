//
// Created by Andrew Chupin on 2019-02-03.
//

#include <thread>
#include "ConnectionPool.h"
#include "../event/MessagePool.h"
#include "../common/Util.h"
#include "../common/Logger.h"

net::ConnectionPool::~ConnectionPool() {
    for (const auto& socket : mActiveConnections) {
        socket->close();
    }

    mQueue.clear();
    mActiveConnections.clear();
}

void net::ConnectionPool::create(const net::NetAddress& address) {
    std::lock_guard<std::mutex> lock(mMutex);
    auto socket = std::make_shared<net::TcpConnection>();
    socket->connect(address);
    socket->attach(mLoop);
    mActiveConnections.push_back(socket);
    if (address.port == 5001) {
        socket->writable();
        async.send();
    }
}

void net::ConnectionPool::asyncCallback(ev::async &watcher, int events) {

}

void net::ConnectionPool::loop() {
    async.set<ConnectionPool, &ConnectionPool::asyncCallback>(this);
    async.start();

    while (true) {
        printf("loop\n");
        mLoop.run(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void net::ConnectionPool::eventLoop() {
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
