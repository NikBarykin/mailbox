#include "test_socket.h"
#include "test_protocol.h"
#include "test_query.h"
#include "test_answer.h"


int main() {
    TestSocket();
    TestProtocol();
    TestAnswer();
    TestQuery();
    return 0;
}
