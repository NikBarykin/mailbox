#include "../socket.h"

#include <string>


namespace Socket {
    class Client;

    class Listener : public Base {
    public:
        Listener(std::string port);

        Client AcceptConnection();

        ~Listener();
    };

    class Client {
        friend Listener;
    private:

        SOCKET socket_;

    };
}