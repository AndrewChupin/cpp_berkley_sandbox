//
// Created by Andrew Chupin on 2019-02-03.
//

#include <thread>
#include "ConnectionPool.h"
#include "../event/MessagePool.h"
#include "../common/Util.h"
#include "../common/Logger.h"
#include "../data/ByteBufferPool.h"

net::ConnectionPool::~ConnectionPool() {
    for (const auto& socket : mActiveConnections) {
        socket->close();
    }

    mActiveConnections.clear();
}

void net::ConnectionPool::create(const net::NetAddress& address) {
    std::lock_guard<std::mutex> lock(mMutex);
    auto socket = std::make_shared<net::TcpConnection>();
    socket->connect(address);
    socket->attach(mLoop);
    mActiveConnections.push_back(socket);
    if (address.port == 5001) {
        auto buf = ByteBufferPool::getInstance()->obtain(); // TODO REMOVE
        socket->enqueue(buf);
        async.send();
    }
}

void net::ConnectionPool::asyncCallback(ev::async &watcher, int events) {
    // TOOD SOME
}

void net::ConnectionPool::loop() {
    async.set<ConnectionPool, &ConnectionPool::asyncCallback>(this);
    async.start();

    while (!isFinished) {
        printf("loop\n");
        mLoop.run(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

