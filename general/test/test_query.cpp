#include "test_query.h"
#include "general/source/query.h"

#include <iostream>
#include <cassert>


namespace {
    void TestGetMail() {
        assert(Query::GetMail{}.SerializeForTransfer() == "GetMail");
        Query::GetMail{}.DeserializeTransfer("GetMail");
    }

    void TestSendMail() {
        Query::SendLetter query{{"A", "bin\ngus", "AB\nO\nBA"}};

        std::string serialized = query.SerializeForTransfer();

        assert(serialized == "SendLetter\nA\nbin\\ngus\nAB\\nO\\nBA");
        assert(query.letter == Query::SendLetter::DeserializeTransfer(serialized).letter);
    }

    void TestAuthorize() {
        Query::Authorize query{"bi\nba", "bo\nba"};

        std::string serialized = query.SerializeForTransfer();
        assert(serialized == "Authorize\nbi\\nba\nbo\\nba");

        Query::Authorize deserialized = Query::Authorize::DeserializeTransfer(serialized);
        assert(deserialized.login == query.login);
        assert(deserialized.password == query.password);
    }

    void TestTerminate() {
        assert(Query::Terminate{}.SerializeForTransfer() == "Terminate");
        Query::Terminate{}.DeserializeTransfer("Terminate");
    }

    void TestGeneralTransfer() {
        std::string serialized1 = Query::SerializeForTransfer(Query::GetMail{});
        assert(serialized1 == "GetMail");

        std::string serialized2 = Query::SerializeForTransfer(Query::SendLetter{{"a", "b", "c"}});
        assert(serialized2 == "SendLetter\na\nb\nc");

        Query::Any deserialized1 = Query::DeserializeTransfer("Authorize\nnikita\nnikita_kot");
        assert(std::holds_alternative<Query::Authorize>(deserialized1));
        assert(std::get<Query::Authorize>(deserialized1).login == "nikita");
        assert(std::get<Query::Authorize>(deserialized1).password == "nikita_kot");

        Query::Any deserialized2 = Query::DeserializeTransfer("Terminate");
        assert(std::holds_alternative<Query::Terminate>(deserialized2));


    }
}


void TestQuery() {
    TestGetMail();
    TestSendMail();
    TestAuthorize();
    TestTerminate();
    TestGeneralTransfer();
    std::cerr << "TestQuery: OK" << std::endl;
}
