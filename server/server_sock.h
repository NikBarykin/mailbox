#include <string>


namespace Socket {
    class Client;

    class Listener {
    private:
        SOCKET socket_;
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