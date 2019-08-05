//
// Created by Andrew Chupin on 2019-08-05.
//

#include "RunLoop.h"
#include "../common/Util.h"
#include "MessagePool.h"


loop::RunLoop::~RunLoop() {
    mQueue.clear();
}

void loop::RunLoop::eventLoop() {
    int64_t now;
    int64_t nextTimeout;
    std::shared_ptr<Message> nextMessage;

    for (;;) {
        LOG_D("loop\n");
        if (isFinished) {
            return;
        }

        nextMessage = mQueue.next();
        now = util::now();
        nextTimeout = def::DEFAULT_EVENT_TIMEOUT;

        if (nextMessage != nullptr) {
            if (now >= nextMessage->time) {
                auto message = mQueue.pop();

                LOG_D("EVENT\n");
                // TODO EVENT

                MessagePool::getInstance()->recycle(message);
                nextMessage = mQueue.next();

                if (nextMessage != nullptr) {
                    nextTimeout = 0;
                }
            } else {
                nextTimeout = nextMessage->time - now;
            }
        }

        if (nextTimeout == 0) {
            continue;
        }

        if (nextTimeout > def::DEFAULT_EVENT_TIMEOUT) {
            nextTimeout = def::DEFAULT_EVENT_TIMEOUT;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(nextTimeout));
    }
}
