#include "common/source/socket.h"
#include "database.h"
#include "server.h"


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    if (argc != 2) {
        std::cout << "Wrong number of command-line arguments, "
                     "should be 1 (servname)" << std::endl;
        return 0;
    }
    Database db;
    Server server(db, argv[1]);
    server.Run();
    return 0;
}
