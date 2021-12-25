#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")


class WSALib {
public:
    WSALib();
    ~WSALib();
};


void Server();
void Client();

namespace Socket {
    // TODO: unit-tests
    // Probably it would be more correct to use SocketView instead of Socket,
    // though we don't really have a socket, we only have its descriptor,
    // but at this point I don't care, I want to used as if it was a complete class that my program own
    // TODO: struct AddrInfo

    struct SocketArgs {
        int af, type, protocol;
    };

    class Base {
        friend void ::Server();
        friend void ::Client();
    protected:
        SOCKET sock_;
        explicit Base(SOCKET);
    public:
        explicit Base(SocketArgs);
        ~Base();
        // TODO: delete copy constructor, though socket descriptor shouldn't be copied
    };

    class Communication : public Base {
    public:
        // TODO: Process string of any size
        static const size_t MAX_STR_SZ = 1024;
    protected:
        explicit Communication(SOCKET);
//        explicit Communication(SocketArgs);
    public:
        std::string Recv();
        void Send(std::string);
    };
}
