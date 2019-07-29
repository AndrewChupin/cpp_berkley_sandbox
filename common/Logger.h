//
// Created by Andrew Chupin on 2019-02-03.
//

#ifndef SOCKET_LOGGER_H
#define SOCKET_LOGGER_H

#import "Definition.h"

#ifdef IS_DEBUG
#define LOG_D(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define LOG_D(...)
#endif

#endif //SOCKET_LOGGER_H
