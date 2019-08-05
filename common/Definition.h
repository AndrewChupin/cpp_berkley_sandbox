//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_DEFINITION_H
#define SOCKET_DEFINITION_H

#include <thread>
#include "../event/Message.h"

#define IS_DEBUG true;

namespace def {

    constexpr int64_t DEFAULT_BUFFER_SIZE = 1024 * 128;
    constexpr int64_t DEFAULT_EVENT_TIMEOUT = 100;

    using MessagePtr = std::shared_ptr<loop::Message>;

}

#endif //SOCKET_DEFINITION_H
