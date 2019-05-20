//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <string>
#include "ByteBuffer.h"
#include "MessageQueue.h"
#include <ev++.h>

class TcpSocket {

private:
    struct IOEvent {
        ev_io io;
        TcpSocket *socket;
    };

    std::string mAddress;
    uint32_t mPort;

    int mSocket;
    struct sockaddr_in mSocketAddress;
    std::shared_ptr<ByteBuffer> buffer;
    IOEvent eventFd;

    void loop();

    static void io(struct ev_loop *loop, ev_io *watcher, int events);
    static void idle(struct ev_loop *loop, ev_idle *watcher, int events);
    static void timeout(struct ev_loop *loop, ev_timer *watcher, int events);

public:

    TcpSocket(std::string address, u_int32_t port);

    TcpSocket(TcpSocket const &) = delete;
    TcpSocket(TcpSocket &&) = default;

    TcpSocket &operator=(TcpSocket const &) = delete;
    TcpSocket &operator=(TcpSocket &&) = default;

    ~TcpSocket();

    void open();
    void destroy();
    bool isConnected();
    void write(uint8_t *data, uint32_t size);
    void write(ByteBuffer &buffer);
};


#endif //SOCKET_SOCKET_H
