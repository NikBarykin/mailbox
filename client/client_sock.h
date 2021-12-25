#pragma once

#include "../socket.h"

#include <string>


namespace Socket {
    class Server : public Communication {
    private:
        static SOCKET MakeConnectionSock(std::string ip, std::string port, SocketArgs);
    public:
        Server(std::string ip, std::string port, SocketArgs = {AF_INET, SOCK_STREAM, IPPROTO_TCP});
    };
}
