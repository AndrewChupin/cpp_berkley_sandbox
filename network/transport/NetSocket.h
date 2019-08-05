//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include <string>
#include "../../event/MessageQueue.h"
#include "../NetAddress.h"
#include "../../data/ByteBuffer.h"
#include "../contract/Socket.h"
#include <ev++.h>
#include <deque>

namespace net {

    enum NetProtocol {
        UDP,
        TCP
    };

    class NetSocket : public Socket {

    private:
        // Host connection
        int32_t mSocket;

        // Host
        NetAddress mAddress;
        NetProtocol mNetProtocol;

        bool hasSocketError();

    public:

        explicit NetSocket(NetAddress address, NetProtocol netProtocol = NetProtocol::TCP);
        ~NetSocket() = default;

        explicit NetSocket(NetSocket const &) = delete;
        NetSocket(NetSocket &&) = delete;

        NetSocket &operator=(NetSocket const &) = delete;
        NetSocket &operator=(NetSocket &&) = delete;

        int32_t descriptor() override;

        int32_t create() override;
        void destroy() override;

        bool isConnected() override;

        bool write(const std::shared_ptr<ByteBuffer>& buf) override;
        bool read(std::shared_ptr<ByteBuffer>& buff) override;
    };
}


#endif //NET_SOCKET_SOCKET_H
