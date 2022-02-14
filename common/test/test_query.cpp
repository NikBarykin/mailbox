#include "test_query.h"
#include "common/source/query.h"
#include "testtools.h"

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
        Query::SendLetter query{{"A", "B", "x\ny"}};

        std::string serialized = query.SerializeForTransfer();

        assert(serialized == "10\nSendLetter1\nA1\nB3\nx\ny");
        assert(query.letter == Query::SendLetter::DeserializeTransfer(serialized).letter);
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
        std::string serialized1 = Query::SerializeForTransfer(Query::GetMail{"body==\"foo\""});
        assert(serialized1 == "7\nGetMail11\nbody==\"foo\"");

        std::string serialized2 = Query::SerializeForTransfer(Query::SendLetter{{"a", "b", "c"}});
        assert(serialized2 == "10\nSendLetter1\na1\nb1\nc");

        Query::Any deserialized1 = Query::DeserializeTransfer("9\nAuthorize6\nnikita10\nnikita_kot");
        assert(std::holds_alternative<Query::Authorize>(deserialized1));
        assert(std::get<Query::Authorize>(deserialized1).login == "nikita");
        assert(std::get<Query::Authorize>(deserialized1).password == "nikita_kot");

        Query::Any deserialized2 = Query::DeserializeTransfer("9\nTerminate");
        assert(std::holds_alternative<Query::Terminate>(deserialized2));

        // Invalid query name

        ASSERT_THROWS(Query::DeserializeTransfer("getmail"), std::exception);
//        try {
//            Query::DeserializeTransfer("getmail");
//            assert(false);
//        } catch (std::exception&) {}
    }

    void TestQueryAnswerDeserialization() {
        std::string serialized1 = "0\n1\na1\nb8\nHey, b!\n";
        auto deserialized1 = Query::DeserializeQueryAnswer(Query::GetMail{}, serialized1);
        assert(std::holds_alternative<Answer::GetMail>(deserialized1));
        std::vector<Letter> expected_mail = {{"a", "b", "Hey, b!\n"}};
        assert(std::get<Answer::GetMail>(deserialized1).mail == expected_mail);

        Query::SendLetter query2 = {{"pupa", "lupa", ":)"}};
        auto deserialized2 = Query::DeserializeQueryAnswer(query2, "0\n");
        assert(std::holds_alternative<Answer::SendLetter>(deserialized2));

        auto deserialized3 = Query::DeserializeQueryAnswer(Query::Terminate{}, "5\nError");
        assert(std::holds_alternative<Answer::Error>(deserialized3));
        assert(std::get<Answer::Error>(deserialized3).message == "Error");
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
