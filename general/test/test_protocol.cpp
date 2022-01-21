#include "test_protocol.h"
#include "general/source/protocol.h"


#include <iostream>
#include <cassert>


namespace {
    void TestQuery() {
        Protocol::Query initial_query{"SendLetter", {"Alice", "Bob", "Hey!"}};
        std::string serialized_query = initial_query.Serialize();
        assert(serialized_query == "10\nSendLetter5\nAlice3\nBob4\nHey!");
        Protocol::Query deserialized_query = Protocol::Query::Deserialize(serialized_query);
        assert(initial_query.name == deserialized_query.name);
        assert(initial_query.arguments == deserialized_query.arguments);
    }

    void TestAnswer() {
        Protocol::Answer initial_answer{"SUS", {"AMO", "GUS"}};
        std::string serialized_answer = initial_answer.Serialize();
        assert(serialized_answer == "3\nSUS3\nAMO3\nGUS");
        Protocol::Answer deserialized_answer = Protocol::Answer::Deserialize(serialized_answer);
        assert(deserialized_answer.error_message == initial_answer.error_message);
        assert(deserialized_answer.fields == initial_answer.fields);
    }
}


void TestProtocol() {
    TestQuery();
    TestAnswer();
    std::cerr << "TestProtocol: OK" << std::endl;
}
