#include "general/source/socket.h"
#include "test_server.h"
#include "test_database.h"
#include "test_query_processor.h"


int main() {
    WSALib wsa_lib;
    TestDatabase();
    TestQueryProcessor();
    TestServer();
    return 0;
}
