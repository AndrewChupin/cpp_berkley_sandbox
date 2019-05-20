//
// Created by Andrew Chupin on 2019-02-03.
//

#include <cstdlib>
#include <algorithm>
#include "ByteBuffer.h"
#include "Logger.h"

ByteBuffer::ByteBuffer() {
    bytes = new uint8_t[DEFAULT_BUFFER_SIZE];
    length = DEFAULT_BUFFER_SIZE;
}

ByteBuffer::ByteBuffer(uint32_t len) {
    bytes = new uint8_t[len];

    if (bytes == nullptr) {
        LOG_D("Cant allocation for byte pointer %d", len);
        exit(1);
    }

    length = len;
}

ByteBuffer::ByteBuffer(uint8_t *buffer, uint32_t len) {
    bytes = new uint8_t[len];

    if (bytes == nullptr) {
        LOG_D("Cant allocation for byte pointer %d", len);
        exit(1);
    }
    length = len;
    std::copy(buffer, buffer + len, bytes);
}

ByteBuffer::ByteBuffer(std::string *data) {
    ssize_t len = strlen(data->c_str());

    bytes = new uint8_t[len];
    length = static_cast<uint32_t>(len);

    std::copy(data->begin(), data->end(), bytes);
}

ByteBuffer::~ByteBuffer() {
    if (bytes != nullptr) {
        delete[] bytes;
        bytes = nullptr;
    }
}

uint32_t ByteBuffer::size() {
    return length;
}