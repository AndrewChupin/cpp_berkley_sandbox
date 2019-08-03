//
// Created by Andrew Chupin on 2019-02-02.
//

#include "SocketConnection.h"
#include "../common/Logger.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <ev.h>

SocketConnection::SocketConnection() {

}

SocketConnection::~SocketConnection() {

}

char message[] = "Hello there!\n";
char buf[sizeof(message)];

void SocketConnection::openConnection() {
    int sock;
    std::string address = "192.168.0.240";

    uint32_t port = 5000;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr =  inet_addr(address.c_str());

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connect\n");
        exit(2);
    }

    uint8_t ad[12];

    send(sock, message, sizeof(ad), 0);
    recv(sock, buf, sizeof(message), 0);
    LOG_D("Hello receive\n");
    close(sock);
}

void SocketConnection::closeConnection() {

}
