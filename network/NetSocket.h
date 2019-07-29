//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <string>
#include "../data/MessageQueue.h"
#include "NetAddress.h"
#include "../data/ByteBuffer.h"
#include <ev++.h>
#include <deque>

namespace net {

    enum NetProtocol {
        UDP,
        TCP
    };

    enum NetFlow {
        ONE_SHOT,
        WRITE_ONLY,
        READ_ONLY,
        BI_SYNC,
        BI_ASYNC
    };

    class NetSocket {

    private:
        // Host
        NetAddress mAddress;
        NetFlow mNetFlow;
        NetProtocol mNetProtocol;

        // Host connection
        int mSocket;

        // Event connection
        ev::default_loop mLoopRead;
        ev::default_loop mLoopWrite;
        ev::io mIo;
        ev::idle mIdleEvent;

        // Message buffer
        std::deque<std::shared_ptr<ByteBuffer>> mBuffers;

        // Sync
        std::mutex mMutex;

        void io(ev::io &watcher, int events);
        void idle(ev::idle &watcher, int events);
        void timeout(ev::timer &watcher, int events);

        void ioWrite(ev::io &watcher);
        void ioRead(ev::io &watcher);

        void updateMode();

    public:
        void loop();

        explicit NetSocket(
                NetAddress address,
                NetProtocol netProtocol = NetProtocol::TCP,
                NetFlow netFlow = NetFlow::ONE_SHOT);
        explicit NetSocket(NetSocket const &) = delete;
        NetSocket(NetSocket &&) = delete;

        NetSocket &operator=(NetSocket const &) = delete;
        NetSocket &operator=(NetSocket &&) = delete;

        ~NetSocket();

        void launch();
        void destroy();
        bool isConnected();
        void write(uint8_t* data, uint32_t size);
        void write(const std::shared_ptr<ByteBuffer>& buf);

        int32_t checkSocketError(int32_t *error);
    };
}


#endif //SOCKET_SOCKET_H
