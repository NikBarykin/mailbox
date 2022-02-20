#include "common/source/socket.h"

#include "test_huffman_compression.h"
#include "test_socket.h"
#include "test_protocol.h"
#include "test_query.h"
#include "test_answer.h"
#include "test_letter.h"


// TODO: make concrete unit-test as independent as possible
//  (i.e. if I change, for example answer.cpp I should only change test_answer.cpp and not test_query)

int main() {
    WSALib wsa_lib;
    TestHuffmanCompression();
    TestSocket();
    TestLetter();
    TestProtocol();
    TestAnswer();
    TestQuery();
    return 0;
}
