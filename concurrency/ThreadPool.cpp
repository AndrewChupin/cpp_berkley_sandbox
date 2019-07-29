//
// Created by Andrew Chupin on 2019-04-06.
//

#include "ThreadPool.h"
#include "BlockingQueue.cpp"

ccrt::ThreadPool::ThreadPool(uint32_t size) : mPoolSize(size) {
    printf("INIT_ThreadPool\n");
    mThreadPool.reserve(mPoolSize);

    for (int i = 0; i < mPoolSize; i++) {
        mThreadPool.emplace_back([&] {
            while (!isStop) {
                auto worker = mWorkerQueue.pop();

                if (worker != nullptr) {
                    worker();
                }
            }
        });
    }
}

ccrt::ThreadPool::~ThreadPool() {
    cancelAndJoin();
}

void ccrt::ThreadPool::execute(Worker&& function) {
    mWorkerQueue.push(function);
}

void ccrt::ThreadPool::cancelAndJoin() {
    isStop = true;
    mWorkerQueue.clear();

    join();
}

void ccrt::ThreadPool::join() {
    for (auto &worker : mThreadPool) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
