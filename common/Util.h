//
// Created by Andrew Chupin on 2019-08-03.
//

#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <chrono>

namespace util {

    uint64_t now() {
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );
        return ms.count();
    }
    
}

#endif //SOCKET_UTIL_H
