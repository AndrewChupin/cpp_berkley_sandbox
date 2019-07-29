//
// Created by Andrew Chupin on 2019-04-06.
//

#ifndef SOCKET_THREADPOOL_H
#define SOCKET_THREADPOOL_H

#include <cstdint>
#include <thread>
#include <array>
#include <vector>
#include "../common/Definition.h"
#include "BlockingQueue.h"


namespace ccrt {
    static const auto DEFAULT_POOL_SIZE = std::thread::hardware_concurrency();

    class ThreadPool {

    private:

        const uint32_t mPoolSize;

        std::vector<std::thread> mThreadPool;
        BlockingQueue<std::function<void()>> mWorkerQueue;

        std::atomic_bool isStop { false };

    public:
        using WorkerQueue = BlockingQueue<std::function<void()>>;
        using Worker = std::function<void()>;

        explicit ThreadPool(uint32_t size);

        ThreadPool(const ThreadPool &) = delete;
        ThreadPool(ThreadPool&& pool) = delete;

        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        ~ThreadPool();

        void execute(Worker&& worker);
        void cancelAndJoin();
        void join();
    };
}


#endif //SOCKET_THREADPOOL_H
