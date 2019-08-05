//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_MESSAGE_H
#define SOCKET_MESSAGE_H

#include <cstdint>
#include "../data/ByteBuffer.h"

namespace loop {

    class Message {

    public:
        ~Message() = default;

        std::shared_ptr<ByteBuffer> data = nullptr;
        time_t time = 0;

        void test();

        // FRIENDS
        friend class MessagePool;

    private:
        Message() = default;

    };

}

#endif //SOCKET_MESSAGE_H
