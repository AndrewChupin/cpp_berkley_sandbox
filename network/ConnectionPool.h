//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_CONNECTIONPOOL_H
#define SOCKET_CONNECTIONPOOL_H

#include <vector>
#include "NetSocket.h"

namespace net {

    class ConnectionPool {

    public:
        ~ConnectionPool();

        void create();
        void loop();

        std::atomic_bool isFinished = { false };

    private:
        std::vector<std::shared_ptr<net::NetSocket>> mSockets;
        MessageQueue mQueue;

    };

}


#endif //SOCKET_CONNECTIONPOOL_H
