//
// Created by Andrew Chupin on 2019-08-05.
//

#include "ByteBufferPool.h"


ByteBufferPool *ByteBufferPool::getInstance() {
    static ByteBufferPool mPool;
    return &mPool;
}

void ByteBufferPool::recycle(const std::shared_ptr<ByteBuffer> &buffer) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (buffer != nullptr) {
        std::memset(buffer->bytes, 0, buffer->size());
        mQueue.push_back(buffer);
    }
}

std::shared_ptr<ByteBuffer> ByteBufferPool::obtain() { // TODO obtain size
    std::lock_guard<std::mutex> lock(mMutex);

    if (mQueue.empty()) {
        return nullptr; // TODO Create new
    }

    auto buffer = mQueue.front();
    mQueue.pop_front();

    return buffer;
}

u_int64_t ByteBufferPool::size() {
    std::lock_guard<std::mutex> lock(mMutex);
    return mQueue.size();
}
