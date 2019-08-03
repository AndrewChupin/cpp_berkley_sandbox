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
#include "../common/Definition.h"
#include "../common/Logger.h"
#include "../data/ByteBuffer.h"

//
// Created by Andrew Chupin on 2019-02-03.
//

net::NetSocket::NetSocket(
        NetAddress address,
        NetProtocol netProtocol
): mAddress(std::move(address)), mNetProtocol(netProtocol), mSocket(-1) {
    LOG_D("INIT_TcpSocket\n");
}

void net::NetSocket::create() {
    if (mNetProtocol == NetProtocol::TCP) {
        mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if (mNetProtocol == NetProtocol::UDP) {
        mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    }

    if (mSocket < 0) {
        LOG_D("ENONBLOCKrror while creating socket\n");
        destroy();
        return;
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
            return;
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
            return;
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
        return;
    }

    if (connect(mSocket, socketAddress, size) == -1 && errno != EINPROGRESS) {
        LOG_D("cannot connect to ip\n");
        destroy();
        return;
    }

    if (socketAddress != nullptr) {
        delete(socketAddress);
    }

    LOG_D("connection created success\n");
}

void net::NetSocket::write(const std::shared_ptr<ByteBuffer>& buf) {
    mBuffers.push_back(buf);
}

void net::NetSocket::write(uint8_t* data, uint32_t size) {
    //mBuffers.emplace_back(data, size);
}

void net::NetSocket::write() {
    while (!mBuffers.empty()) {

        auto buff = mBuffers.front();
        mBuffers.pop_front();

        ssize_t writeResult = send(mSocket, buff->bytes, buff->size(), 0);

        if (writeResult >= 0) {
            LOG_D("Sent message with size: %zi\n", writeResult);
            LOG_D("Sent message content: %s\n", buff->bytes);
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", writeResult);
            close(mSocket);
            return;
        } else {
            LOG_D("Error while sending%zi\n", writeResult);
            close(mSocket);
            return;
        }
    }
}

void net::NetSocket::read() {
    auto* readBuffer = new ByteBuffer();

    ssize_t readResult = recv(mSocket, readBuffer->bytes, def::DEFAULT_BUFFER_SIZE, 0);

    if (readResult >= 0) {
        LOG_D("Receive message with size: %zi\n", readResult);
        LOG_D("Received message content: %s\n", readBuffer->bytes);

        close(mSocket);
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_D("Error while reading EAGAIN or EWOULDBLOCK %zi\n", readResult);
        close(mSocket);
    } else {
        LOG_D("Error while reading %zi\n", readResult);
        close(mSocket);
    }

    delete readBuffer;
}

bool net::NetSocket::isConnected() {
    return mSocket > 0;
}

void net::NetSocket::destroy() {
    if (mSocket > 0) {
        if (close(mSocket) != 0) {
            LOG_D("socket already closed\n");
        }
    }
}

int32_t net::NetSocket::checkSocketError() {
    if (mSocket < 0) {
        return true;
    }
    int ret;
    int code;
    socklen_t len = sizeof(int);
    ret = getsockopt(mSocket, SOL_SOCKET, SO_ERROR, &code, &len);
    if (ret != 0 || code != 0) {
        LOG_D("socket error 0x%x code 0x%x", ret, code);
    }
    return (ret || code) != 0;
}
