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
        Listener(std::string port, SocketArgs, int backlog = SOMAXCONN);

        Client Accept();
    };
}
