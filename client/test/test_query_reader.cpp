#include "test_query_reader.h"
#include "client/source/query_reader.h"


#include <iostream>
#include <cassert>
#include <sstream>


namespace {
    void Test() {
        SessionState sst{.user_login = "NikitaKot"};
        std::istringstream input(R"(GetMail
SendLetter
BestFriend
Yo, Bro
What da dog doin?
\End-of-letter
Authorize
Biba
Boba
Terminate
InvalidQueryName)");
        std::ostringstream output;
        QueryReader query_reader(sst, input, output);

        Query::Any query1 = query_reader();
        assert(std::holds_alternative<Query::GetMail>(query1));

        Query::Any query2 = query_reader();
        assert(std::holds_alternative<Query::SendLetter>(query2));
        Letter expected_letter{"NikitaKot", "BestFriend", "Yo, Bro\nWhat da dog doin?"};
        assert(std::get<Query::SendLetter>(query2).letter == expected_letter);

        Query::Any query3 = query_reader();
        assert(std::holds_alternative<Query::Authorize>(query3));
        auto [login, password] = std::get<Query::Authorize>(query3);
        assert(login == "Biba" && password == "Boba");

        Query::Any query4 = query_reader();
        assert(std::holds_alternative<Query::Terminate>(query4));

        // Query name is invalid
        try {
            query_reader();
            assert(false);
        } catch(std::exception&) {}

        // Input is empty
        try {
            query_reader();
            assert(false);
        } catch(std::exception&) {}

        std::string expected_output = R"(Query name:

Query name:
Letter recipient:
Letter body:

Query name:
Login:
Password:

Query name:

Query name:
Query name:
)";
        assert(output.str() == expected_output);
    }
}


void TestQueryReader() {
    Test();
    std::cerr << "TestQueryReader: OK" << std::endl;
}
