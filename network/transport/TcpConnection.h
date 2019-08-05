//
// Created by Andrew Chupin on 2019-08-03.
//

#ifndef SOCKET_TCPCONNECTION_H
#define SOCKET_TCPCONNECTION_H

#include <deque>
#include "../NetAddress.h"
#include "../../data/ByteBuffer.h"
#include "NetSocket.h"

namespace net {

    class TcpConnection {

    private:
        // Message buffer
        std::unique_ptr<net::NetSocket> mSocket = nullptr;
        std::deque<std::shared_ptr<ByteBuffer>> mWriteQueue;
        ev::io mIo;

        void ioCallback(ev::io &watcher, int32_t events);
        void innerWriteCallback();
        void innerReadCallback();

    public:
        bool connect(const net::NetAddress& address);
        void close();

        void attach(ev::default_loop& loop);
        void enqueue(std::shared_ptr<ByteBuffer>& buffer);
    };

}


#endif //SOCKET_TCPCONNECTION_H
