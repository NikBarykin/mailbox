#include <string>


class WSASetuper {
    WSASetuper() {
        WSADATA wsaData;
        WSASetup(MAKEWORD(2,2), &wsaData);
    }
    ~WSASetuper() {
        WSACleanup();
    }
};


namespace Socket {
    class Base {
    protected:
        SOCKET sock_;
    public:
        ~BaseSocket() {
            closesocket(sock_);
        }
    };

    class Communication {
        std::string Recv();
        void Send(std::string);
    };
}