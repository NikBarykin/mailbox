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
        explicit Listener(std::string port, SocketArgs = {AF_INET, SOCK_STREAM, IPPROTO_TCP}, int backlog = SOMAXCONN);

        Client Accept();
    };
}
