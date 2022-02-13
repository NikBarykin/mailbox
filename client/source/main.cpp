#include "common/source/socket.h"
#include "session.h"

#include <iostream>


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    // TODO: throw exception in case of invalid number of command-line arguments
    RunSession(argv[1], argv[2], std::cin, std::cout);
    return 0;
}
