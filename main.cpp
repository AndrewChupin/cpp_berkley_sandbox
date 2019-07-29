#include <iostream>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "trash/SocketConnection.h"
#include "common/Logger.h"
#include "network/NetSocket.h"
#include "concurrency/ThreadPool.h"
#include <ev++.h>
#include <fstream>



int main(int argc, char **argv) {
    ccrt::ThreadPool pool { 3 };

    net::NetAddress address { net::NetAddress::V4("192.168.0.240", 5000) };
    net::NetSocket socket { address, net::NetProtocol::TCP, net::NetFlow::ONE_SHOT };

    pool.execute([&] {
        std::string message = "Single socket message";
        socket.launch();
        socket.write(std::make_shared<ByteBuffer>(&message));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    pool.execute([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        socket.loop();
    });


    /*while (true) {
        std::string str;
        puts("Enter text:");
        std::getline(std::cin, str);
        socket.write(std::make_shared<ByteBuffer>(&str));
    }*/
    pool.join();
}