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


namespace Socket {
    // Probably it would be more elegant to use SocketView instead of Socket,
    // though we don't really have a socket, we only have its descriptor,
    // but at this point I don't care, I want to use it as if it was a complete class that my program own
    // TODO: struct AddrInfo

    struct SocketArgs {
        int af, type, protocol;
    };

    class Base {
    protected:
        SOCKET sock_;
        explicit Base(SOCKET);
    public:
        explicit Base(SocketArgs);
        ~Base();

        Base(Base&& other);

        Base(const Base&) = delete;
        Base& operator=(const Base&) = delete;
        Base& operator=(Base&&) = delete;
    };

    class Communicator : public Base {
    protected:
        explicit Communicator(SOCKET);
    private:
        void SendSize(const std::string &);
        size_t RecvSize();

    public:
        void Send(std::string, bool compress = false, bool encrypt = false);
        std::string Recv();
    };
}
