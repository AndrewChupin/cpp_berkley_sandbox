//
// Created by Andrew Chupin on 2019-04-06.
//

#include "BlockingQueue.h"


template<typename T>
void BlockingQueue<T>::push(T &&t) {
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.emplace_back(t);
    }

    mCondition.notify_one();
}

template<typename T>
T BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mMutex);
    mCondition.wait(lock, [&] { return isStop || !mQueue.empty(); });

    if (isStop || mQueue.empty()) {
        return nullptr;
    }

    auto rc = this->mQueue.front();
    mQueue.pop_back();
    return rc;
}


template<typename T>
void BlockingQueue<T>::clear() {
    mQueue.clear();
    isStop = true;
    mCondition.notify_all();
}
