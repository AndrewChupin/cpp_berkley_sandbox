//
// Created by Andrew Chupin on 2019-04-06.
//

#include "BlockingQueue.h"


template<typename T>
void ccrt::BlockingQueue<T>::push(T& t) {
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mQueue.push_back(t);
    }

    mCondition.notify_one();
}
template<typename T>
T& ccrt::BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mMutex);
    mCondition.wait(lock, [&] { return isStop || !mQueue.empty(); });

    T& rc = this->mQueue.front();
    mQueue.pop_front();
    return rc;
}


template<typename T>
void ccrt::BlockingQueue<T>::clear() {
    mQueue.clear();
    isStop = true;
    mCondition.notify_all();
}


template<typename T>
ccrt::BlockingQueue<T>::~BlockingQueue() {
    printf("~BlockingQueue\n");
}

template<typename T>
ccrt::BlockingQueue<T>::BlockingQueue() {
    printf("BlockingQueue\n");
}
