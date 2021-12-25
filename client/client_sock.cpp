#include "client_sock.h"

#include <stdexcept>


namespace Socket {
    SOCKET Server::MakeConnectionSock(std::string nodename, std::string servname, SocketArgs sock_args) {
        addrinfo* addr_info = nullptr, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = sock_args.af;
        hints.ai_socktype = sock_args.type;
        hints.ai_protocol = sock_args.protocol;
        int i_result = getaddrinfo(nodename.c_str(), servname.c_str(), &hints, &addr_info);
        if (i_result != 0) {
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(i_result));
        }
        SOCKET connect_sock = INVALID_SOCKET;
        for (addrinfo* ptr = addr_info; ptr != nullptr && connect_sock == INVALID_SOCKET; ptr = ptr->ai_next) {
            connect_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (connect_sock == INVALID_SOCKET) {
                throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
            }
            i_result = connect(connect_sock, ptr->ai_addr, ptr->ai_addrlen);
            if (i_result == SOCKET_ERROR) {
                closesocket(connect_sock);
                connect_sock = INVALID_SOCKET;
            }
        }
        freeaddrinfo(addr_info);
        if (connect_sock == INVALID_SOCKET) {
            throw std::runtime_error("failed to connect");
        }
        return connect_sock;
    }

    Server::Server(std::string ip, std::string port, SocketArgs sock_args)
    : Communication(MakeConnectionSock(ip, port, sock_args)) {}
}
