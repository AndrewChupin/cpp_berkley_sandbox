//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_BYTEBUFFER_H
#define SOCKET_BYTEBUFFER_H

#include <cstdint>
#include <string>

class ByteBuffer {

public:
    ByteBuffer();
    explicit ByteBuffer(uint32_t length);
    ByteBuffer(uint8_t *buffer, uint32_t len);
    explicit ByteBuffer(std::string *data);
    ~ByteBuffer();

    uint8_t *bytes;
    uint32_t size();

private:
    uint32_t length;
};


#endif //SOCKET_BYTEBUFFER_H
