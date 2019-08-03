//
// Created by Andrew Chupin on 2019-08-02.
//

#ifndef SOCKET_NETFLOW_H
#define SOCKET_NETFLOW_H

namespace net {

    enum NetFlow {
        ONE_SHOT,
        WRITE_ONLY,
        READ_ONLY,
        BI_SYNC,
        BI_ASYNC
    };
}

#endif //SOCKET_NETFLOW_H
