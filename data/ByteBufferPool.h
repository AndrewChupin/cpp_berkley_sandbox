//
// Created by Andrew Chupin on 2019-08-05.
//

#ifndef SOCKET_BYTEBUFFERPOOL_H
#define SOCKET_BYTEBUFFERPOOL_H


#include <memory>
#include <deque>
#include <mutex>
#include "ByteBuffer.h"

class ByteBufferPool {

public:
    ByteBufferPool(const ByteBufferPool&) = delete;
    ByteBufferPool& operator= (const ByteBufferPool&) = delete;

    ByteBufferPool(const ByteBufferPool&&) = delete;
    ByteBufferPool& operator= (ByteBufferPool&&) = delete;

    static ByteBufferPool* getInstance();

    void recycle(const std::shared_ptr<ByteBuffer>& buffer);
    std::shared_ptr<ByteBuffer> obtain();
    u_int64_t size();

private:
    ByteBufferPool() = default;
    ~ByteBufferPool() = default;

    std::mutex mMutex;
    std::deque<std::shared_ptr<ByteBuffer>> mQueue;


};


#endif //SOCKET_BYTEBUFFERPOOL_H
