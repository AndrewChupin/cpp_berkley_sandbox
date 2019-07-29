#include <utility>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <zconf.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "NetSocket.h"
#include "../common/Definition.h"
#include "../common/Logger.h"
#include "../data/ByteBuffer.h"

//
// Created by Andrew Chupin on 2019-02-03.
//

net::NetSocket::NetSocket(
        NetAddress address,
        NetProtocol netProtocol,
        NetFlow mNetFlow
): mAddress(std::move(address)), mNetProtocol(netProtocol), mNetFlow(mNetFlow), mSocket(-1) {
    LOG_D("INIT_TcpSocket\n");
}

net::NetSocket::~NetSocket() = default;

bool net::NetSocket::isConnected() {
    return mSocket > 0;
}

void net::NetSocket::destroy() {
    if (mSocket > 0) {
        if (close(mSocket) != 0) {
            LOG_D("socket already closed\n");
        }
    }
}

int32_t net::NetSocket::checkSocketError(int32_t *error) {
    if (mSocket < 0) {
        return true;
    }
    int ret;
    int code;
    socklen_t len = sizeof(int);
    ret = getsockopt(mSocket, SOL_SOCKET, SO_ERROR, &code, &len);
    if (ret != 0 || code != 0) {
        LOG_D("socket error 0x%x code 0x%x", ret, code);
    }
    *error = code;
    return (ret || code) != 0;
}

void net::NetSocket::launch() {
    if ((mSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_D("Error while creating socket\n");
        destroy();
        return;
    }

    struct sockaddr* socketAddress = nullptr;
    size_t size = 0;
    if (mAddress.ipType == net::IpType::V4) {
        sockaddr_in addressV4 { 0 };
        addressV4.sin_family = AF_INET;
        addressV4.sin_port = htons(mAddress.port);
        addressV4.sin_addr.s_addr = inet_addr(mAddress.ip.c_str());

        if (inet_pton(AF_INET, mAddress.ip.c_str(), &(addressV4.sin_addr.s_addr)) != 1) {
            LOG_D("Connection not ipv4\n");
            return;
        }

        socketAddress = reinterpret_cast<sockaddr*>(&addressV4);
        size = sizeof(addressV4);
    } else if (mAddress.ipType == net::IpType::V6) {
        sockaddr_in6 addressV6 { 0 };
        addressV6.sin6_family = AF_INET6;
        addressV6.sin6_port = htons(mAddress.port);
        addressV6.sin6_flowinfo = 0;
        addressV6.sin6_scope_id = 0;
        memcpy(addressV6.sin6_addr.s6_addr, mAddress.ip.c_str(), 16);

        if (inet_pton(AF_INET6, mAddress.ip.c_str(), &(addressV6.sin6_addr.s6_addr)) != 1) {
            LOG_D("Connection not ipv6\n");
            return;
        }

        socketAddress = reinterpret_cast<sockaddr*>(&addressV6);
        size = sizeof(addressV6);
    }


    int yes = 1;
    if (mNetProtocol == NetProtocol::TCP) {
        if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int))) {
            LOG_D("connection set TCP_NODELAY failed\n");
        }
    }
    else if (mNetProtocol == NetProtocol::UDP) {
        // TODO UDP SOCKET
    }

    /*struct timeval timeout {};
    timeout.tv_usec = 0;
    timeout.tv_sec = 30;
    setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    timeout.tv_sec = 30;
    setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));*/

    if (fcntl(mSocket, F_SETFL, O_NONBLOCK) == -1) {
        LOG_D("connection set O_NONBLOCK failed\n");
        destroy();
        return;
    }

    if (connect(mSocket, socketAddress, size) == -1 && errno != EINPROGRESS) {
        LOG_D("cannot connect to ip\n");
        destroy();
        return;
    }

    //delete(socketAddress);

    LOG_D("connection created success\n");

    /*if (mNetFlow == net::NetFlow::UNI) {
        loop();
    }
    else if (mNetFlow == net::NetFlow::BI) {
        loop();
    }*/
}

void net::NetSocket::write(const std::shared_ptr<ByteBuffer>& buf) {
    //std::unique_lock<std::mutex> lock(mMutex);
    //lock.unlock();

    //mBuffers.push_back(buf);
    mIo.set<NetSocket, &NetSocket::io>(this);
    mIo.start(mSocket, ev::READ);
    mLoopRead.loop(0);
}

void net::NetSocket::io(ev::io &watcher, int events) {
    LOG_D("io_cb\n");
    if (events & EV_WRITE) {
        LOG_D("EV_WRITE\n");
        ioWrite(watcher);
    } else if (events & EV_READ) {
        LOG_D("EV_READ\n");
        ioRead(watcher);
    }

    if (mBuffers.empty()) {
        mIo.set(ev::READ);
    }
}

void net::NetSocket::ioWrite(ev::io &watcher) {
    while (!mBuffers.empty()) {

        auto buff = mBuffers.front();
        mBuffers.pop_front();

        ssize_t writeResult = send(watcher.fd, buff->bytes, buff->size(), 0);

        if (writeResult >= 0) {
            LOG_D("Sent message with size: %zi\n", writeResult);
            LOG_D("Sent message content: %s\n", buff->bytes);
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", writeResult);
            mLoopWrite.break_loop();
            close(mSocket);
            return;
        } else {
            LOG_D("Error while sending%zi\n", writeResult);
            mLoopWrite.break_loop();
            close(mSocket);
            return;
        }
    }
}

void net::NetSocket::ioRead(ev::io &watcher) {
    auto* readBuffer = new ByteBuffer();

    ssize_t readResult = recv(watcher.fd, readBuffer->bytes, DEFAULT_BUFFER_SIZE, 0);

    if (readResult >= 0) {
        LOG_D("Receive message with size: %zi\n", readResult);
        LOG_D("Received message content: %s\n", readBuffer->bytes);

        watcher.stop();
        mLoopWrite.break_loop();
        close(mSocket);
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", readResult);
        watcher.stop();
        mLoopRead.break_loop();
        close(mSocket);
    } else {
        LOG_D("Error while reading %zi\n", readResult);
        watcher.stop();
        mLoopRead.break_loop();
        close(mSocket);
    }

    delete readBuffer;
}

















// NOT USED
void net::NetSocket::loop() {
    //mIo.set<NetSocket, &NetSocket::io>(this);
    mIo.set(mSocket, ev::WRITE);
    mLoopRead.now();
    LOG_D("loop started success\n");
   // mLoopRead.loop(0);
}

void net::NetSocket::updateMode() {
    if (mBuffers.empty()) {
        mIo.set(ev::READ);
    } else {
        mIo.set(ev::READ | ev::WRITE);
    }
}

void net::NetSocket::idle(ev::idle &watcher, int events) {
    if (events & EV_ERROR) {
        LOG_D("idle EV_ERROR %d\n", events);
        exit(12);
    }

    if (events & EV_IDLE) {
        // printf("idle EV_IDLE %d\n", events);
        updateMode();
    }
}

void net::NetSocket::timeout(ev::timer &watcher, int events) {
    LOG_D("timeout events %d\n", events);

    // Idle
    // mIdleEvent.set<NetSocket, &NetSocket::idle>(this);
    // mIdleEvent.start();

    // Timeout
    // ev_timer timeout_watcher;
    // ev_timer_init(&timeout_watcher, timeout, 5, 1);
    // ev_timer_start(mLoop, &timeout_watcher);
}


void net::NetSocket::write(uint8_t* data, uint32_t size) {
    //mBuffers.emplace_back(data, size);
}