#include "general/source/socket.h"
#include "test_query_reader.h"
#include "test_answer_processor.h"
#include "test_session.h"


int main() {
    WSALib wsa_lib;
    TestQueryReader();
    TestAnswerProcessor();
    TestSession();
    return 0;
}
