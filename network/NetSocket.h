//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include <string>
#include "../event/MessageQueue.h"
#include "NetAddress.h"
#include "../data/ByteBuffer.h"
#include "contract/Socket.h"
#include <ev++.h>
#include <deque>

namespace net {

    enum NetProtocol {
        UDP,
        TCP
    };

    class NetSocket : public Socket {

    private:
        // Host
        NetAddress mAddress;
        NetProtocol mNetProtocol;

        // Host connection
        int mSocket;

        // Message buffer
        std::deque<std::shared_ptr<ByteBuffer>> mBuffers;

        void write();
        void read();

    public:
        explicit NetSocket(NetAddress address, NetProtocol netProtocol = NetProtocol::TCP);
        ~NetSocket() = default;

        explicit NetSocket(NetSocket const &) = delete;
        NetSocket(NetSocket &&) = delete;

        NetSocket &operator=(NetSocket const &) = delete;
        NetSocket &operator=(NetSocket &&) = delete;


        void create();
        void destroy();

        bool isConnected() override;

        void write(uint8_t* data, uint32_t size) override;
        void write(const std::shared_ptr<ByteBuffer>& buf) override;

        int32_t checkSocketError();
    };
}


#endif //NET_SOCKET_SOCKET_H
