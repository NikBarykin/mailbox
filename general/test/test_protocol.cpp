#include "test_protocol.h"
#include "general/source/protocol.h"


#include <iostream>
#include <cassert>


namespace {
    void TestQuery() {
        Protocol::Query initial_query{"SendLetter",
                                      {"Alice", "Bob", "Hey, Bob\nHow do you do?\nMiss you\nAlice"}};
        std::string serialized_query = initial_query.Serialize();
        assert(serialized_query == "SendLetter\nAlice\nBob\nHey, Bob\\nHow do you do?\\nMiss you\\nAlice");
        Protocol::Query deserialized_query = Protocol::Query::Deserialize(serialized_query);
        assert(initial_query.name == deserialized_query.name);
        assert(initial_query.arguments == deserialized_query.arguments);
    }

    void TestAnswer() {
        Protocol::Answer initial_answer{"A\n\nBA", "C\nA\nBA"};
        std::string serialized_answer = initial_answer.Serialize();
        assert(serialized_answer == "A\\n\\nBA\nC\\nA\\nBA");
        Protocol::Answer deserialized_answer = Protocol::Answer::Deserialize(serialized_answer);
        assert(deserialized_answer.error_message == initial_answer.error_message);
        assert(deserialized_answer.body == initial_answer.body);
    }
}


void TestProtocol() {
    TestQuery();
    TestAnswer();
    std::cerr << "TestProtocol: OK" << std::endl;
}
