#include "test_build_query.h"
#include "client/source/build_query.h"


#include <iostream>
#include <cassert>
#include <sstream>


namespace {
    void Test() {
        SessionState sst{.user_login = "NikitaKot"};
        std::istringstream input(R"(GetMail
from == "BestFriend" || body == ":]"
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

        Query::GetMail query1 = std::get<Query::GetMail>(BuildQuery(input, output, sst));
        assert(query1.letter_filter == R"(from == "BestFriend" || body == ":]")");

        Query::Any query2 = BuildQuery(input, output, sst);
        assert(std::holds_alternative<Query::SendLetter>(query2));
        Letter expected_letter{"NikitaKot", "BestFriend", "Yo, Bro\nWhat da dog doin?"};
        assert(std::get<Query::SendLetter>(query2).letter == expected_letter);

        Query::Any query3 = BuildQuery(input, output, sst);
        assert(std::holds_alternative<Query::Authorize>(query3));
        auto [login, password] = std::get<Query::Authorize>(query3);
        assert(login == "Biba" && password == "Boba");

        Query::Any query4 = BuildQuery(input, output, sst);
        assert(std::holds_alternative<Query::Terminate>(query4));

        // Query name is invalid
        try {
            BuildQuery(input, output, sst);
            assert(false);
        } catch(std::exception&) {}

        // Input is empty
        try {
            BuildQuery(input, output, sst);
            assert(false);
        } catch(std::exception&) {}

        std::string expected_output = R"(Query name:
Letter filter:

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


void TestBuildQuery() {
    Test();
    std::cerr << "TestBuildQuery: OK" << std::endl;
}
