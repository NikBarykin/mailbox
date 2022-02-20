#include "testing_utility.h"
#include "test_query.h"
#include "common/source/query.h"

#include <iostream>
#include <cassert>
#include <stdexcept>


namespace {
    void TestGetMail() {
        Query::GetMail query{"from != \"xxx\""};

        std::string serialized = query.SerializeForTransfer();

        assert(serialized == "7\nGetMail13\nfrom != \"xxx\"");
        assert(query.letter_filter == Query::GetMail::DeserializeTransfer(serialized).letter_filter);
    }

    void TestSendMail() {
        Query::SendLetter query{"A B C"_l};

        std::string serialized = query.SerializeForTransfer();

//        assert(serialized == "10\nSendLetter1\nA1\nB1\nC");
        assert(query == Query::SendLetter::DeserializeTransfer(serialized));
    }

    void TestAuthorize() {
        Query::Authorize query{"bi\nba", "bo\nba"};

        std::string serialized = query.SerializeForTransfer();
        assert(serialized == "9\nAuthorize5\nbi\nba5\nbo\nba");

        Query::Authorize deserialized = Query::Authorize::DeserializeTransfer(serialized);
        assert(deserialized.login == query.login);
        assert(deserialized.password == query.password);
    }

    void TestTerminate() {
        assert(Query::Terminate{}.SerializeForTransfer() == "9\nTerminate");
        Query::Terminate{}.DeserializeTransfer("9\nTerminate");
    }

    void TestGeneralTransfer() {
        auto check_query = [](Query::Any q) {
            std::string serialized = Query::SerializeForTransfer(q);
            assert(Query::DeserializeTransfer(serialized) == q);
        };

        check_query(Query::GetMail{"body==\"foo\""});
        check_query(Query::SendLetter{"a b c"_l});
        check_query(Query::Authorize{"nikita", "nikita_kot"});
        check_query(Query::Terminate{});

        // Invalid query name

        ASSERT_THROWS(Query::DeserializeTransfer("aboba"), std::exception);
    }

    void TestQueryAnswerDeserialization() {
        auto check_answer_deserialization = [](const Query::Any &query, const Answer::Any &ans) {
            std::string serialized = Answer::SerializeForTransfer(ans);
            assert(Query::DeserializeQueryAnswer(query, serialized) == ans);
        };

        check_answer_deserialization(Query::GetMail{}, Answer::GetMail{{"1 2 3"_l,
                                                                        "$ % @"_l}});

        check_answer_deserialization(Query::SendLetter{"pupa lupa :)"_l}, Answer::SendLetter{});

        // Error occurred
        check_answer_deserialization(Query::Terminate{}, Answer::Error{"Fatal!"});
    }
}


void TestQuery() {
    TestGetMail();
    TestSendMail();
    TestAuthorize();
    TestTerminate();
    TestGeneralTransfer();
    TestQueryAnswerDeserialization();
    std::cerr << "TestQuery: OK" << std::endl;
}
