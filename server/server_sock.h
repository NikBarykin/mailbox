#pragma once

#include "../socket.h"

#include <string>


namespace Socket {
    class Listener;

    class Client : public Communication {
        friend Listener;
    private:
        explicit Client(SOCKET sock);
    };

    class Listener : public Base {
    public:
        // TODO: maybe Socket::Base as and argument
        explicit Listener(std::string port, int af = AF_INET, int type = SOCK_STREAM,
                          int protocol = IPPROTO_TCP, int backlog = SOMAXCONN);

        Client Accept();
    };
}
