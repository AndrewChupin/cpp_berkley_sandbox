#include <iostream>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "SocketConnection.h"
#include "Logger.h"
#include "TcpSocket.h"
#include "concurrency/ThreadPool.h"
#include <ev++.h>
#include <fstream>
#include <SLE>


class One {
private:
    explicit One(int a) {
    }

public:

    One(One const &) = delete;
    One(One &&) = default;

    One &operator=(One const &) = delete;
    One &operator=(One &&) = default;

    ~One() {

    }

    static One create() {
        One one {3};
        return one;
    }
};


int main(int argc, char **argv) {
    ThreadPool pool { 3 };

    pool.execute([] {
        TcpSocket socket("192.168.0.240", 5000);
        socket.open();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    while (true) {
        std::string str;
        std::getline(std::cin, str);

    }
}