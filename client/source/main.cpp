#include "common/source/socket.h"
#include "session.h"

#include <iostream>


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    if (argc != 3) {
        std::cout << "Wrong number of command-line arguments, "
                     "should be 2 (nodename, servname)" << std::endl;
        return 0;
    }
    RunSession(argv[1], argv[2], std::cin, std::cout);
    return 0;
}
