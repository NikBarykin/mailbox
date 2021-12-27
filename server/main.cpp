#include "interaction.h"
#include "../socket.h"
#include "server.h"

using namespace std;


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    Server server;
    server.Run();
    return 0;
}
