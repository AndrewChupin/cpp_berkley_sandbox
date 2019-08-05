//
// Created by Andrew Chupin on 2019-08-02.
//

#include "MessagePool.h"
#include "../common/Definition.h"
#include "../common/Logger.h"

loop::MessagePool* loop::MessagePool::getInstance() {
    static MessagePool mPool;
    return &mPool;
}

void loop::MessagePool::recycle(const def::MessagePtr& message) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (message != nullptr) {
        message->data = nullptr;
        message->time = 0;
        mQueue.push_back(message);
        LOG_D("MessagePool_recycle NOTNULL size %lu\n", mQueue.size());
    } else {
        LOG_D("MessagePool_recycle NULL size %lu\n", mQueue.size());
    }
}

def::MessagePtr loop::MessagePool::obtain() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mQueue.empty()) {
        LOG_D("MessagePool_obtain new Message\n");
        auto* message = new Message();
        return std::shared_ptr<Message>(message);
    }

    LOG_D("MessagePool_obtain reuse Message\n");

    auto front = mQueue.front();
    mQueue.pop_front();

    return front;
}

u_int64_t loop::MessagePool::size() {
    std::lock_guard<std::mutex> lock(mMutex);
    return mQueue.size();
}
