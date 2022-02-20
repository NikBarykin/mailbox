#include "test_build_query.h"
#include "client/source/build_query.h"
#include "common/test/testing_utility.h"

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
XXX

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
        Query::Any expected2 = Query::SendLetter{"NikitaKot BestFriend XXX"_l};
        assert(query2 == expected2);

        Query::Any query3 = BuildQuery(input, output, sst);
        assert(std::holds_alternative<Query::Authorize>(query3));
        auto [login, password] = std::get<Query::Authorize>(query3);
        assert(login == "Biba" && password == "Boba");

        Query::Any query4 = BuildQuery(input, output, sst);
        assert(std::holds_alternative<Query::Terminate>(query4));

        // Input is empty
        ASSERT_THROWS(BuildQuery(input, output, sst), std::exception);

        std::string expected_output = R"(Query name:
Letter filter:

Query name:
Letter addressee:
Letter body: (empty line marks end of letter)

Query name:
Login:
Password:

Query name:

Query name:
Unknown query, type "Help" to get list of queries
Query name:
)";
        assert(output.str() == expected_output);
    }
}


void TestBuildQuery() {
    Test();
    std::cerr << "TestBuildQuery: OK" << std::endl;
}
