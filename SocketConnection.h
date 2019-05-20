//
// Created by Andrew Chupin on 2019-02-02.
//

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>
#include <string>

class SocketConnection {
public:
    SocketConnection();
    ~SocketConnection();

    void openConnection();
    void closeConnection();

private:
    struct sockaddr_in addr;
};


#endif //SOCKET_SERVER_H
