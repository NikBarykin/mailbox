#include "server_sock.h"

#include <stdexcept>


namespace Socket {
    Client::Client(SOCKET sock): Communication(sock) {}


    Listener::Listener(std::string port, SocketArgs sock_args, int backlog): Base(sock_args) {
        addrinfo *addr_info = nullptr, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = sock_args.af;
        hints.ai_socktype = sock_args.type;
        hints.ai_protocol = sock_args.protocol;
        int i_result = getaddrinfo(nullptr, port.c_str(), &hints, &addr_info);
        if (i_result != 0) {
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(i_result));
        }
        i_result = bind(sock_, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen));
        freeaddrinfo(addr_info);
        if (i_result != 0) {
            throw std::runtime_error("bind failed: " + std::to_string(i_result));
        }
        i_result = listen(sock_, backlog);
        if (i_result != 0) {
            throw std::runtime_error("listen failed: " + std::to_string(i_result));
        }
    }

    Client Listener::Accept() {
        SOCKET client_sock = accept(sock_, nullptr, nullptr);
        return Client(client_sock);
    }
}
