#include "socket.h"

#include <stdexcept>
#include <iostream>


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

    Base::Base(Base &&other) {
        sock_ = INVALID_SOCKET;
        std::swap(sock_, other.sock_);
    }


    Communication::Communication(SOCKET sock): Base(sock) {}

    std::string Communication::Recv() {
        char sz_bytes[sizeof(size_t)];
        int i_recv_result = recv(sock_, sz_bytes, sizeof(size_t), 0);
        if (i_recv_result < 0) {
            throw std::runtime_error("data size recv error: " + std::to_string(WSAGetLastError()));
        }
        size_t sz = *static_cast<size_t *>(static_cast<void *>(sz_bytes));

        char data[sz];
        i_recv_result = recv(sock_, data, sz, 0);
        if (i_recv_result < 0) {
            throw std::runtime_error("data recv error: " + std::to_string(WSAGetLastError()));
        }
        return {data, data + sz};
    }

    void Communication::Send(std::string data) {
        char sz_bytes[sizeof(size_t)];
        *static_cast<size_t *>(static_cast<void *>(sz_bytes)) = data.size();
        int i_send_result = send(sock_, sz_bytes, sizeof(size_t), 0);
        if (i_send_result == SOCKET_ERROR) {
            throw std::runtime_error("data size send error: " + std::to_string(WSAGetLastError()));
        }

        i_send_result = send(sock_, data.c_str(), data.size(), 0);
        if (i_send_result == SOCKET_ERROR) {
            throw std::runtime_error("data send error: " + std::to_string(WSAGetLastError()));
        }
    }
}
