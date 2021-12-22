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
    class Base {
    protected:
        SOCKET sock_;
    public:
        Base(int af, int type, int protocol);
        ~Base();
    };

    class Communication : public Base {
    public:
        std::string Recv() {
            socket()
        }
        void Send(std::string);
    };
}
