#include "../../socket.h"
#include "database.h"
#include "server.h"

using namespace std;


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    Database db;
    Server server(db);
    server.Run();
    return 0;
}
