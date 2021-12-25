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

//    Communication::Communication(SocketArgs sock_args): Base(sock_args) {}

    std::string Communication::Recv() {
        char data[MAX_STR_SZ] = {0};
        int i_recv_result = recv(sock_, data, strlen(data), 0);
        if (i_recv_result < 0) {
            throw std::runtime_error("recv error: " + std::to_string(WSAGetLastError()));
        }
        return data;
    }

    void Communication::Send(std::string data) {
        if (data.size() > MAX_STR_SZ) {
            throw std::runtime_error("failed to send, string is too large, its size "
                                     "(" + std::to_string(data.size()) + ") is greater than maximal allowed size "
                                                                         "(" + std::to_string(MAX_STR_SZ) + ")");
        }
        int i_send_result = send(sock_, data.c_str(), data.size(), 0);
        if (i_send_result == SOCKET_ERROR) {
            throw std::runtime_error("send error: " + std::to_string(WSAGetLastError()));
        }
    }
}
