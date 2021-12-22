#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")


class WSALib {
    WSALib();
    ~WSALib();
};


namespace Socket {
    // Probably it would be more correct to use SocketView instead of Socket,
    // though we don't really have a socket, we only have its descriptor,
    // but at this point I don't care, I want to used as if it was a complete class that my programm own
    // TODO: struct AddrInfo
    class Base {
    protected:
        SOCKET sock_;
        Base(SOCKET sock);
    public:
        // TODO: maybe af, type, protocol -> struct SocketArgs,
        //  though it is boring to type 3 variables in constructor every time you want to inherit new socket type
        Base(int af, int type, int protocol);
        ~Base();
        // TODO: delete copy constructor, though socket descriptor shouldn't be copied
    };

    class Communication : public Base {
    public:
        std::string Recv() {
            socket()
        }
        void Send(std::string);
    };
}
