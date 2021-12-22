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

    Base::Base(int af, int type, int protocol) {
        sock_ = socket(af, type, protocol);
        if (sock_ == INVALID_SOCKET) {
            throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
        }
    }

    Base::~Base() {
        closesocket(sock_);
    }
}
