#include <utility>

//
// Created by Andrew Chupin on 2019-07-27.
//

#ifndef SOCKET_NETADDRESS_H
#define SOCKET_NETADDRESS_H

#include <string>
#include "../common/Logger.h"

namespace net {

    enum IpType {
        V4,
        V6
    };

    class NetAddress {
    public:
        std::string ip;
        int32_t port;
        IpType ipType;

        NetAddress(const NetAddress&) = default;
        NetAddress& operator=(const NetAddress&) = default;

        NetAddress(NetAddress&&) = default;
        NetAddress& operator=(NetAddress&&) = default;

        static net::NetAddress v4(const std::string &ip, int32_t port) {
            return NetAddress(ip, port, IpType::V4);
        }

        static net::NetAddress v6(const std::string &ip, int32_t port) {
            return NetAddress(ip, port, IpType::V6);
        }

    private:
        NetAddress(std::string ip, int32_t port, IpType ipType) : ip(std::move(ip)), port(port), ipType(ipType) {
            LOG_D("init NetAddress\n");
        }
    };
}


#endif //SOCKET_NETADDRESS_H
