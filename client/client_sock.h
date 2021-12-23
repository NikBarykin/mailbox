#pragma once

#include "../socket.h"

#include <string>


namespace Socket {
    class Server : public Communication {
    public:
        explicit Server(SocketArgs);
    };
}
