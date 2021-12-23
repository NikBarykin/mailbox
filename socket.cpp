#include "socket.h"

#include <stdexcept>


WSALib::WSALib()  {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
}

WSALib::~WSALib()  {
    WSACleanup();
}


namespace Socket {
    Base::Base(SOCKET sock): sock_(sock) {
        if (sock_ == INVALID_SOCKET) {
            throw std::runtime_error("INVALID_SOCKET construction is forbidden");
        }
    }

    Base::Base(SocketArgs sock_args) {
        sock_ = socket(sock_args.af, sock_args.type, sock_args.protocol);
        if (sock_ == INVALID_SOCKET) {
            throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
        }
    }

    Base::~Base() {
        closesocket(sock_);
    }


    Communication::Communication(SOCKET sock): Base(sock) {}

    Communication::Communication(SocketArgs sock_args): Base(sock_args) {}
}
