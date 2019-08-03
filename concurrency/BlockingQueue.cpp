//
// Created by Andrew Chupin on 2019-04-06.
//

#include "BlockingQueue.h"
#include "../common/Logger.h"


template<typename T>
void conc::BlockingQueue<T>::push(T& t) {
    std::lock_guard<std::mutex> lock(mMutex);
    mQueue.push_back(t);
    mCondition.notify_one();
}

template<typename T>
T& conc::BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mMutex);
    mCondition.wait(lock, [&] { return isStop || !mQueue.empty(); });

    T& rc = this->mQueue.front();
    mQueue.pop_front();
    return rc;
}


template<typename T>
void conc::BlockingQueue<T>::clear() {
    std::unique_lock<std::mutex> lock(mMutex);
    mQueue.clear();
    isStop = true;
    mCondition.notify_all();
}


template<typename T>
conc::BlockingQueue<T>::~BlockingQueue() {
    LOG_D("~BlockingQueue\n");
}

template<typename T>
conc::BlockingQueue<T>::BlockingQueue() {
    LOG_D("BlockingQueue\n");
}
