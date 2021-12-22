#include "server_sock.h"

#include <stdexcept>


namespace Socket {
    Client::Client(SOCKET sock): Communication(sock) {}


    Listener::Listener(std::string port, int af, int type, int protocol,
                       int backlog): Base(af, type, protocol) {
        addrinfo *addr_info = nullptr, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = af;
        hints.ai_socktype = type;
        hints.ai_protocol = protocol;
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
