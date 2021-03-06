#pragma once

#include "common/source/socket.h"

#include <string>


namespace Socket {
    class Listener;

    class Client : public Communicator {
        friend Listener;
    private:
        explicit Client(SOCKET sock);
    };

    class Listener : public Base {
    public:
        // TODO: maybe Socket::Base as an argument
        explicit Listener(std::string servname, SocketArgs = {AF_INET, SOCK_STREAM, IPPROTO_TCP},
                          int backlog = SOMAXCONN);

        Client Accept();
    };
}
