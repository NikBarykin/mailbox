#include "socket.h"
#include "huffman_compression.h"

#include <stdexcept>
#include <iostream>
#include <cassert>


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


    Communicator::Communicator(SOCKET sock): Base(sock) {}

    void Communicator::SendSize(const std::string & data) {
        char sz_bytes[sizeof(size_t)];
        *static_cast<size_t *>(static_cast<void *>(sz_bytes)) = data.size();
        int i_send_result = send(sock_, sz_bytes, sizeof(size_t), 0);
        if (i_send_result == SOCKET_ERROR) {
            throw std::runtime_error("data size send error: " + std::to_string(WSAGetLastError()));
        }
    }

    void Communicator::Send(std::string data, bool compress, bool encrypt) {
        if (compress) {
            data = PerformHuffmanCompression(data);
        }

        if (encrypt) {
            // TODO: encryption
            throw std::runtime_error("Encryption isn't implemented!");
        }
        // compress and encrypt flags
        // TODO: make flags more elegant. For example by adding section "flags"
        data = std::string(compress ? "c" : "C") + std::string(encrypt ? "e" : "E") + data;

        SendSize(data);

        int i_send_result = send(sock_, data.c_str(), data.size(), 0);
        if (i_send_result == SOCKET_ERROR) {
            throw std::runtime_error("data send error: " + std::to_string(WSAGetLastError()));
        }
    }

    size_t Communicator::RecvSize() {
        char sz_bytes[sizeof(size_t)];
        int i_recv_result = recv(sock_, sz_bytes, sizeof(size_t), 0);
        if (i_recv_result < 0) {
            throw std::runtime_error("data size recv error: " + std::to_string(WSAGetLastError()));
        }
        return *static_cast<size_t *>(static_cast<void *>(sz_bytes));
    }

    // TODO: decryption
    std::string Communicator::Recv() {
        size_t sz = RecvSize();

        char buf[sz];
        int i_recv_result = recv(sock_, buf, sz, 0);
        if (i_recv_result < 0) {
            throw std::runtime_error("data recv error: " + std::to_string(WSAGetLastError()));
        }

        assert(sz >= 2);
        // data without flags
        std::string data{buf + 2, buf + sz};

        if (buf[1] == 'e') {
            // TODO: decryption
            throw std::runtime_error("Decryption isn't implemented!");
        }

        if (buf[0] == 'c') {
            data = PerformHuffmanDecompression(data);
        }

        return data;
    }
}
