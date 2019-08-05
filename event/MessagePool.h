//
// Created by Andrew Chupin on 2019-08-02.
//

#ifndef SOCKET_MESSAGEPOOL_H
#define SOCKET_MESSAGEPOOL_H

#include <mutex>
#include <deque>
#include "Message.h"
#include "../common/Logger.h"


namespace loop {

    class MessagePool {

    public:

        MessagePool(const MessagePool&) = delete;
        MessagePool& operator= (const MessagePool&) = delete;

        MessagePool(const MessagePool&&) = delete;
        MessagePool& operator= (MessagePool&&) = delete;

        static MessagePool* getInstance();

        void recycle(const def::MessagePtr& message);
        std::shared_ptr<loop::Message> obtain();
        u_int64_t size();

    private:
        MessagePool() = default;
        ~MessagePool() = default;

        std::mutex mMutex;
        std::deque<std::shared_ptr<Message>> mQueue;

    };

}


#endif //SOCKET_MESSAGEPOOL_H
