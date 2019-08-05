//
// Created by Andrew Chupin on 2019-08-03.
//

#include "TcpConnection.h"
#include "NetSocket.h"


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

void net::TcpConnection::writable() {
    mIo.set(ev::WRITE | ev::READ);
    mIo.loop.feed_fd_event(mSocket->descriptor(), ev::WRITE);
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
    mSocket->write(std::make_shared<ByteBuffer>(&message));
    mIo.set(ev::READ);
}

void net::TcpConnection::innerReadCallback() {
    auto buffer = std::make_shared<ByteBuffer>();
    mSocket->read(buffer);
}
