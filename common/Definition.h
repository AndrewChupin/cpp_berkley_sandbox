//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_DEFINITION_H
#define SOCKET_DEFINITION_H

#include <thread>

#define IS_DEBUG true;

namespace def {

    constexpr int64_t DEFAULT_BUFFER_SIZE = 1024 * 128;
    constexpr int64_t DEFAULT_EVENT_TIMEOUT = 100;

}

#endif //SOCKET_DEFINITION_H
