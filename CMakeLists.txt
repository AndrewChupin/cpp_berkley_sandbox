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
        SocketConnection.cpp
        SocketConnection.h
        TcpSocket.cpp
        TcpSocket.h
        Connection.cpp
        Connection.h
        ConnectionPool.cpp
        ConnectionPool.h
        Logger.h
        Definition.h
        Message.cpp
        Message.h
        ByteBuffer.cpp
        ByteBuffer.h
        Request.cpp
        Request.h
        Config.cpp
        Config.h
        MessageQueue.h
        MessageQueue.cpp
        concurrency/BlockingQueue.cpp
        concurrency/BlockingQueue.h
        concurrency/ThreadPool.cpp
        concurrency/ThreadPool.h)

link_directories(/usr/local/Cellar/libev/4.24/lib)
target_link_libraries(socket /usr/local/Cellar/libev/4.24/lib/libev.a)