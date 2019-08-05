#include <iostream>
#include "concurrency/ThreadPool.h"
#include "network/ConnectionPool.h"


int main(int argc, char **argv) {
    //

    /*pool.execute([&] {
        //socket.create();
        //socket.write(std::make_shared<ByteBuffer>(&message));
    });*/


    /*while (true) {
        std::string str;
        puts("Enter text:");
        std::getline(std::cin, str);
        socket.write(std::make_shared<ByteBuffer>(&str));
    }*/

    //pool.join();

    // net::NetAddress address { net::NetAddress::V4("192.168.0.240", 5001) };
    // net::NetSocket socket { address, net::NetProtocol::TCP };

    conc::ThreadPool pool { 3 };
    auto conPool = std::make_shared<net::ConnectionPool>();

    pool.execute([conPool] {
        conPool->loop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    conPool->create(net::NetAddress::v4("192.168.0.240", 5001));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    conPool->create(net::NetAddress::v4("192.168.0.240", 5002));

    pool.join();
}