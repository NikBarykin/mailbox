#include "client_sock.h"

#include <stdexcept>


namespace Socket {
    SOCKET Server::MakeConnectionSock(std::string ip, std::string port, SocketArgs sock_args) {
        addrinfo* addr_info = nullptr, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = sock_args.af;
        hints.ai_socktype = sock_args.type;
        hints.ai_protocol = sock_args.protocol;
        int i_result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &addr_info);
        if (i_result != 0) {
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(i_result));
        }
        for (addrinfo* ptr = addr_info; ptr != nullptr; ptr = ptr->ai_next) {
            SOCKET connect_sock = socket(sock_args.af, sock_args.type, sock_args.protocol);
            if (connect_sock == INVALID_SOCKET) {
                throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
            }
            i_result = connect(connect_sock, ptr->ai_addr, ptr->ai_addrlen);
            if (i_result == 0) {
                return connect_sock;
            }
        }
        return INVALID_SOCKET;
    }

    Server::Server(std::string ip, std::string port, SocketArgs sock_args)
    : Communication(MakeConnectionSock(ip, port, sock_args)) {}
}
