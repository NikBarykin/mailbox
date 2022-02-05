#include "common/source/socket.h"
#include "test_server.h"
#include "test_database.h"
#include "test_query_processor.h"
#include "test_letter_filter.h"

int main() {
    WSALib wsa_lib;
    TestLetterFilter();
    TestDatabase();
    TestQueryProcessor();
    TestServer();
    return 0;
}
