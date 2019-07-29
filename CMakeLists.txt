cmake_minimum_required(VERSION 3.13)
project(socket)


# set the path to the library folder
# set(libev_DIR "/usr/local/Cellar/libev/4.24")
# find_package(libev NAMES ev.h)
# link_directories(/usr/local/lib)
# set(LINK_LIB_NAMES libev.a)

# find_package(${LINK_LIB_NAMES} REQUIRED)
set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/libev/4.24/include)

add_executable(socket
        main.cpp
        trash/SocketConnection.cpp
        trash/SocketConnection.h
        network/NetSocket.cpp
        network/NetSocket.h
        trash/Connection.cpp
        trash/Connection.h
        trash/ConnectionPool.cpp
        trash/ConnectionPool.h
        common/Logger.h
        common/Definition.h
        data/Message.cpp
        data/Message.h
        data/ByteBuffer.cpp
        data/ByteBuffer.h
        trash/Request.cpp
        trash/Request.h
        data/MessageQueue.h
        data/MessageQueue.cpp
        concurrency/BlockingQueue.cpp
        concurrency/BlockingQueue.h
        concurrency/ThreadPool.cpp
        concurrency/ThreadPool.h network/NetAddress.h)

link_directories(/usr/local/Cellar/libev/4.24/lib)
target_link_libraries(socket /usr/local/Cellar/libev/4.24/lib/libev.a)