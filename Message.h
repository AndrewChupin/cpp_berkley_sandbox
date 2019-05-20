//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_MESSAGE_H
#define SOCKET_MESSAGE_H


#include <cstdint>
#include "ByteBuffer.h"

struct Message {
    ByteBuffer *buffer;
};


#endif //SOCKET_MESSAGE_H
