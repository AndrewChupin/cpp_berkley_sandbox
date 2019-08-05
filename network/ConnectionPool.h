//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_CONNECTIONPOOL_H
#define SOCKET_CONNECTIONPOOL_H

#include <vector>
#include "transport/NetSocket.h"
#include "transport/TcpConnection.h"

namespace net {

    class ConnectionPool {

    public:
        ~ConnectionPool();

        void create(const net::NetAddress& address);
        void loop();
        void eventLoop();
        void asyncCallback(ev::async &watcher, int events);

        std::atomic_bool isFinished = { false };

    private:
        // Connections
        std::vector<std::shared_ptr<net::TcpConnection>> mActiveConnections;

        // Sync
        MessageQueue mQueue;
        std::mutex mMutex;


        ev::default_loop mLoop;
        ev::async async;
    };

}


#endif //SOCKET_CONNECTIONPOOL_H
