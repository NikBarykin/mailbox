#include "test_query_processor.h"
#include "../source/server.h"
#include "../source/query_processor.h"

#include <iostream>
#include <cassert>


namespace {
    void TestEverything() {
        SessionState session_state;
        Database db;
        QueryProcessor query_processor(db, session_state);
        Answer::Any ans;
        ans = query_processor(Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans));
        ans = query_processor(Query::SendLetter{Letter{"a", "b", "c"}});
        assert(std::holds_alternative<Answer::Error>(ans));
    }
}

void TestQueryProcessor() {

    std::cerr << "TestQueryProcessor: OK" << std::endl;
}
