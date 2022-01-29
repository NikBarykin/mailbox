#include "common/source/socket.h"
#include "session.h"

#include <iostream>


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    RunSession(argv[1], argv[2], std::cin, std::cout);
    return 0;
}
