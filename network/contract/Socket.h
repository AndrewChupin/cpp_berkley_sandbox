//
// Created by Andrew Chupin on 2019-08-02.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <memory>
#include "../../data/ByteBuffer.h"

class Socket {

public:
    virtual bool isConnected() = 0;
    virtual void write(uint8_t* data, uint32_t size) = 0;
    virtual void write(const std::shared_ptr<ByteBuffer>& buf) = 0;

};

#endif //SOCKET_SOCKET_H
