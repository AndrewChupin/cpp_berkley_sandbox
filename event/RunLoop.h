//
// Created by Andrew Chupin on 2019-08-05.
//

#ifndef SOCKET_RUNLOOP_H
#define SOCKET_RUNLOOP_H


#include "MessageQueue.h"

namespace loop {

    class RunLoop {

    private:
        loop::MessageQueue mQueue;
        std::atomic_bool isFinished = { false };

    public:
        RunLoop() = default;
        ~RunLoop();

        void eventLoop();

    };

}

#endif //SOCKET_RUNLOOP_H
