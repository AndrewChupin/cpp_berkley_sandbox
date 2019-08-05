#include <utility>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <zconf.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "NetSocket.h"
#include "../../common/Definition.h"
#include "../../common/Logger.h"
#include "../../data/ByteBuffer.h"

//
// Created by Andrew Chupin on 2019-02-03.
//

net::NetSocket::NetSocket(
        NetAddress address,
        NetProtocol netProtocol
): mAddress(std::move(address)), mNetProtocol(netProtocol), mSocket(-1) {
    LOG_D("INIT_TcpSocket\n");
}

int32_t net::NetSocket::create() {
    if (mNetProtocol == NetProtocol::TCP) {
        mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    else if (mNetProtocol == NetProtocol::UDP) {
        mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    }

    if (mSocket < 0) {
        LOG_D("ENONBLOCKrror while creating socket\n");
        destroy();
        return -1;
    }

    struct sockaddr* socketAddress = nullptr;
    size_t size = 0;
    if (mAddress.ipType == net::IpType::V4) {
        sockaddr_in addressV4 { 0 };
        addressV4.sin_family = AF_INET;
        addressV4.sin_port = htons(mAddress.port);
        addressV4.sin_addr.s_addr = inet_addr(mAddress.ip.c_str());

        if (inet_pton(AF_INET, mAddress.ip.c_str(), &(addressV4.sin_addr.s_addr)) != 1) {
            LOG_D("Connection not ipv4\n");
            return -1;
        }

        socketAddress = reinterpret_cast<sockaddr*>(&addressV4);
        size = sizeof(addressV4);
    } else if (mAddress.ipType == net::IpType::V6) {
        sockaddr_in6 addressV6 { 0 };
        addressV6.sin6_family = AF_INET6;
        addressV6.sin6_port = htons(mAddress.port);
        addressV6.sin6_flowinfo = 0;
        addressV6.sin6_scope_id = 0;
        memcpy(addressV6.sin6_addr.s6_addr, mAddress.ip.c_str(), 16);

        if (inet_pton(AF_INET6, mAddress.ip.c_str(), &(addressV6.sin6_addr.s6_addr)) != 1) {
            LOG_D("Connection not ipv6\n");
            return -1;
        }

        socketAddress = reinterpret_cast<sockaddr*>(&addressV6);
        size = sizeof(addressV6);
    }


    if (mNetProtocol == NetProtocol::TCP) {
        int yes = 1;
        if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int))) {
            LOG_D("connection set TCP_NODELAY failed\n");
        }
    }

    if (fcntl(mSocket, F_SETFL, O_NONBLOCK) == -1) {
        LOG_D("connection set O_NONBLOCK failed\n");
        destroy();
        return -1;
    }

    if (connect(mSocket, socketAddress, size) == -1 && errno != EINPROGRESS) {
        LOG_D("cannot connect to ip\n");
        destroy();
        return -1;
    }

    //if (socketAddress != nullptr) {
    //    delete(socketAddress);
    //}

    LOG_D("connection created success\n");
    return 0;
}

void net::NetSocket::destroy() {
    if (mSocket > 0) {
        if (close(mSocket) != 0) {
            LOG_D("socket already closed\n");
        }
    }
}

int32_t net::NetSocket::descriptor() {
    return mSocket;
}

bool net::NetSocket::write(const std::shared_ptr<ByteBuffer>& buf) {
    LOG_D("net::NetSocket::write\n");

    if (!isConnected()) {
        return false;
    }

    LOG_D("net::NetSocket::write and connected\n");

    ssize_t writeResult = send(mSocket, buf->bytes, buf->size(), 0);

    if (writeResult >= 0) {
        LOG_D("Sent message with size: %zi\n", writeResult);
        LOG_D("Sent message content: %s\n", buf->bytes);
        return true;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", writeResult);
        destroy();
    } else {
        LOG_D("Error while sending%zi\n", writeResult);
        destroy();
    }

    return false;
}


bool net::NetSocket::read(std::shared_ptr<ByteBuffer>& buff) {
    LOG_D("net::NetSocket::read\n");

    if (!isConnected()) {
        return false;
    }

    LOG_D("net::NetSocket::read and connected\n");

    auto buffer = std::make_shared<ByteBuffer>();

    ssize_t readResult = recv(mSocket, buffer->bytes, def::DEFAULT_BUFFER_SIZE, 0);

    if (readResult >= 0) {
        LOG_D("Receive message with size: %zi\n", readResult);
        LOG_D("Received message content: %s\n", buffer->bytes);
        return true;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", readResult);
        destroy();
    } else {
        LOG_D("Error while reading %zi\n", readResult);
        destroy();
    }

    return false;
}

bool net::NetSocket::hasSocketError() {
    int ret;
    int code;
    socklen_t len = sizeof(int);
    ret = getsockopt(mSocket, SOL_SOCKET, SO_ERROR, &code, &len);

    if (ret != 0 || code != 0) {
        LOG_D("socket error 0x%x code 0x%x", ret, code);
    }

    return (ret || code) != 0;
}

bool net::NetSocket::isConnected() {
    bool hasError = hasSocketError();

    if (hasError) {
        destroy();
    }

    return !hasError;
}
