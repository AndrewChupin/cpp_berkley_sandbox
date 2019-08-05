//
// Created by Andrew Chupin on 2019-08-02.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <memory>
#include "../../data/ByteBuffer.h"

class Socket {

public:
    virtual int32_t descriptor() = 0;

    virtual int32_t create() = 0;
    virtual void destroy() = 0;

    virtual bool isConnected() = 0;
    virtual bool write(const std::shared_ptr<ByteBuffer>& buf) = 0;
    virtual bool read(std::shared_ptr<ByteBuffer>& buff) = 0;

};

#endif //SOCKET_SOCKET_H
