#include <utility>

#include <utility>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <zconf.h>
#include <thread>
#include <fstream>
#include <iostream>

//
// Created by Andrew Chupin on 2019-02-03.
//

#include "TcpSocket.h"
#include "Definition.h"
#include "Logger.h"


TcpSocket::TcpSocket(std::string address, u_int32_t port): mAddress(std::move(address)), mPort(port) {
    LOG_D("INIT_TcpSocket\n");
    buffer = std::make_shared<ByteBuffer>();
}

TcpSocket::~TcpSocket() = default;

void TcpSocket::open() {
    if ((mSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_D("Error while creating socket");
        destroy();
        return;
    }

    mSocketAddress.sin_family = AF_INET;
    mSocketAddress.sin_port = htons(mPort);
    mSocketAddress.sin_addr.s_addr = inet_addr(mAddress.c_str());

    if (inet_pton(AF_INET, mAddress.c_str(), &mSocketAddress.sin_addr.s_addr) != 1) {
        LOG_D("Connection not ipv4 address ");
        return;
    }

    int yes = 1;
    if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int))) {
        LOG_D("connection set TCP_NODELAY failed");
    }

    if (fcntl(mSocket, F_SETFL, O_NONBLOCK) == -1) {
        LOG_D("connection set O_NONBLOCK failed");
        destroy();
        return;
    }

    if (connect(mSocket, (struct sockaddr *) &mSocketAddress, sizeof(mSocketAddress)) == -1 && errno != EINPROGRESS) {
        LOG_D("cannot connect for address");
        destroy();
        return;
    }

    LOG_D("connection created success %d", 1);
    loop();
    LOG_D("connection created success");
}


void TcpSocket::destroy() {
    if (mSocket > 0) {
        if (close(mSocket) != 0) {
            LOG_D("socket already closed");
        }
    }
}


bool TcpSocket::isConnected() {
    return mSocket > 0;
}


void TcpSocket::write(uint8_t *data, uint32_t size) {

}


void TcpSocket::write(ByteBuffer &buffer) {

}


void TcpSocket::loop() {
    // ev_idle idleIO;
    // ev_timer timeout_watcher;
    eventFd.socket = this;

    auto loop = ev_default_loop(0);

    ev_io_init(&eventFd.io, io, mSocket, EV_READ);
    ev_io_start(loop, &eventFd.io);

    // ev_timer_init (&timeout_watcher, timeout, 1, 1);
    // ev_timer_start(loop, &timeout_watcher);

    // ev_idle_init(&idleIO, idle);
    // ev_idle_start(loop, &idleIO);

    ev_loop(loop, 0);
}


#pragma ide diagnostic ignored "OCDFAInspection"
void TcpSocket::io(struct ev_loop *loop, ev_io *watcher, int events) {
    ev_io_stop(loop, watcher);
    auto* eventFd = (IOEvent*) watcher;

    if (events & EV_WRITE) {
        ev_io_set(watcher, watcher->fd, EV_READ);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string message = "Hello from client";
        ssize_t writeResult = send(watcher->fd, message.c_str(), message.length(), 0);

        if (writeResult >= 0) {
            LOG_D("Sent message with size: %zi\n", writeResult);
            LOG_D("Sent message content: %s\n", message.c_str());
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", writeResult);
            ev_loop_destroy(loop);
            // TODO CLOSE CONNECTION
            return;
        } else {
            LOG_D("Error while sending%zi\n", writeResult);
            ev_loop_destroy(loop);
            // TODO CLOSE CONNECTION
            return;
        }

        ev_io_start(loop, watcher);
    } else if (events & EV_READ) {
        ev_io_set(watcher, watcher->fd, EV_WRITE);

        auto *readBuffer = new ByteBuffer();

        ssize_t readResult = recv(watcher->fd, readBuffer->bytes, DEFAULT_BUFFER_SIZE, 0);

        if (readResult >= 0) {
            LOG_D("Receive message with size: %zi\n", readResult);
            LOG_D("Received message content: %s\n", readBuffer->bytes);
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", readResult);
        } else {
            LOG_D("Error while reading%zi\n", readResult);
        }

        delete readBuffer;

        ev_io_start(loop, watcher);
    }
}


void TcpSocket::idle(struct ev_loop *loop, ev_idle *watcher, int events) {
    if (events & EV_ERROR) {
        LOG_D("idle EV_ERROR %d\n", events);
        exit(12);
    }

    if (events & EV_IDLE) {
        printf("idle EV_IDLE %d\n", events);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}


void TcpSocket::timeout(struct ev_loop *loop, ev_timer *watcher, int events) {
    LOG_D("timeout events %d\n", events);
}
