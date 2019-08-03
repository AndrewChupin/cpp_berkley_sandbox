//
// Created by Andrew Chupin on 2019-04-06.
//

#include "ThreadPool.h"
#include "BlockingQueue.cpp"
#include "../common/Logger.h"

conc::ThreadPool::ThreadPool(uint32_t size) : mPoolSize(size) {
    LOG_D("INIT_ThreadPool\n");
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

conc::ThreadPool::~ThreadPool() {
    cancelAndJoin();
}

void conc::ThreadPool::execute(Worker&& function) {
    mWorkerQueue.push(function);
}

void conc::ThreadPool::cancelAndJoin() {
    isStop = true;
    mWorkerQueue.clear();

    join();
}

void conc::ThreadPool::join() {
    for (auto &worker : mThreadPool) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
