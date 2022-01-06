#include "test_query.h"
#include "general/source/query.h"

#include <iostream>
#include <cassert>
#include <stdexcept>


namespace {
    void TestGetMail() {
        assert(Query::GetMail{}.SerializeForTransfer() == "GetMail\n");
        Query::GetMail{}.DeserializeTransfer("GetMail");
    }

    void TestSendMail() {
        Query::SendLetter query{{"A", "bin\ngus", "AB\nO\nBA"}};

        std::string serialized = query.SerializeForTransfer();

        assert(serialized == "SendLetter\nA\nbin\\ngus\nAB\\nO\\nBA\n");
        assert(query.letter == Query::SendLetter::DeserializeTransfer(serialized).letter);
    }

    void TestAuthorize() {
        Query::Authorize query{"bi\nba", "bo\nba"};

        std::string serialized = query.SerializeForTransfer();
        assert(serialized == "Authorize\nbi\\nba\nbo\\nba\n");

        Query::Authorize deserialized = Query::Authorize::DeserializeTransfer(serialized);
        assert(deserialized.login == query.login);
        assert(deserialized.password == query.password);
    }

    void TestTerminate() {
        assert(Query::Terminate{}.SerializeForTransfer() == "Terminate\n");
        Query::Terminate{}.DeserializeTransfer("Terminate");
    }

    void TestGeneralTransfer() {
        std::string serialized1 = Query::SerializeForTransfer(Query::GetMail{});
        assert(serialized1 == "GetMail\n");

        std::string serialized2 = Query::SerializeForTransfer(Query::SendLetter{{"a", "b", "c"}});
        assert(serialized2 == "SendLetter\na\nb\nc\n");

        Query::Any deserialized1 = Query::DeserializeTransfer("Authorize\nnikita\nnikita_kot");
        assert(std::holds_alternative<Query::Authorize>(deserialized1));
        assert(std::get<Query::Authorize>(deserialized1).login == "nikita");
        assert(std::get<Query::Authorize>(deserialized1).password == "nikita_kot");

        Query::Any deserialized2 = Query::DeserializeTransfer("Terminate");
        assert(std::holds_alternative<Query::Terminate>(deserialized2));

        // Invalid query name
        try {
            Query::DeserializeTransfer("getmail");
            assert(false);
        } catch (std::exception&) {}
    }

    void TestQueryAnswerDeserialization() {
        std::string serialized1 = "\na\nb\nHey, b!\\nLove, a";
        auto deserialized1 = Query::DeserializeQueryAnswer(Query::GetMail{}, serialized1);
        assert(std::holds_alternative<Answer::GetMail>(deserialized1));
        std::vector<Letter> expected_mail = {{"a", "b", "Hey, b!\nLove, a"}};
        assert(std::get<Answer::GetMail>(deserialized1).mail == expected_mail);

        Query::SendLetter query2 = {{"pupa", "lupa", ":)"}};
        auto deserialized2 = Query::DeserializeQueryAnswer(query2, "");
        assert(std::holds_alternative<Answer::SendLetter>(deserialized2));

        auto deserialized3 = Query::DeserializeQueryAnswer(Query::Terminate{}, "Error");
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
