//
// Created by Andrew Chupin on 2019-04-06.
//

#ifndef SOCKET_BLOCKINGQUEUE_H
#define SOCKET_BLOCKINGQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>

namespace conc {
    template<typename T>
    class BlockingQueue {

    private:
        std::mutex mMutex;
        std::condition_variable mCondition;
        std::deque<T> mQueue;
        std::atomic_bool isStop { false };

    public:
        BlockingQueue();
        ~BlockingQueue();

        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue& operator=(const BlockingQueue&) = delete;

        void push(T& t);
        T& pop();
        void clear();
    };

}

#endif //SOCKET_BLOCKINGQUEUE_H
