#pragma once

#include "general/source/socket.h"

#include <string>


namespace Socket {
    class Server : public Communicator {
    private:
        static SOCKET MakeConnectionSock(std::string nodename, std::string servname, SocketArgs);
    public:
        Server(std::string nodename, std::string servname, SocketArgs = {AF_INET, SOCK_STREAM, IPPROTO_TCP});
    };
}
