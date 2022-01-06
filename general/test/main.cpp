#include "general/source/socket.h"
#include "test_socket.h"
#include "test_protocol.h"
#include "test_query.h"
#include "test_answer.h"


int main() {
    WSALib wsa_lib;
    TestSocket();
    TestProtocol();
    TestAnswer();
    TestQuery();
    return 0;
}
