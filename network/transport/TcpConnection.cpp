//
// Created by Andrew Chupin on 2019-08-03.
//

#include "TcpConnection.h"
#include "NetSocket.h"
#include "../../data/ByteBufferPool.h"


bool net::TcpConnection::connect(const net::NetAddress& address) {
    mSocket = std::make_unique<net::NetSocket>(address);

    int32_t result = mSocket->create();
    return result >= 0;
}

void net::TcpConnection::close() {
    mSocket->destroy();
}

void net::TcpConnection::attach(ev::default_loop& loop) {
    mIo.set<TcpConnection, &TcpConnection::ioCallback>(this);
    mIo.start(mSocket->descriptor(), ev::READ);
}

void net::TcpConnection::enqueue(std::shared_ptr<ByteBuffer>& buffer) {
    mWriteQueue.push_back(buffer);
    mIo.set(ev::WRITE | ev::READ);
}

void net::TcpConnection::ioCallback(ev::io &watcher, int32_t events) {
    if (events & ev::READ) {
        innerReadCallback();
    } else if (events & ev::WRITE) {
        innerWriteCallback();
    }
}

void net::TcpConnection::innerWriteCallback() {
    std::string message = "Hello from C++";
    while (!mWriteQueue.empty()) {
        auto buffer = mWriteQueue.front();
        bool isSuccess = mSocket->write(buffer); // TODO BUFFER SET DATA
        if (isSuccess) {
            mWriteQueue.pop_front();
            ByteBufferPool::getInstance()->recycle(buffer);
        } // TODO fail
    }
    mIo.set(ev::READ);
}

void net::TcpConnection::innerReadCallback() {
    auto buffer = ByteBufferPool::getInstance()->obtain();
    bool isSuccess = mSocket->read(buffer);
    if (isSuccess) {
        ByteBufferPool::getInstance()->recycle(buffer);
    } // TODO fail
}
