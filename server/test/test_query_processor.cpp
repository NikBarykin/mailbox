#include "test_query_processor.h"
#include "../source/server.h"
#include "../source/query_processor.h"
#include "general/test/test_tools.h"

#include <iostream>
#include <cassert>


namespace {
    void TestGetMailProcessing() {
        SessionState session_state;
        Database db;
        QueryProcessor query_processor(db, session_state);
        // Not authorized
        Answer::Any ans1 = query_processor(Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans1));
        session_state.user_id = *db.Authorize("a", "a");
        Answer::Any ans2 = query_processor(Query::GetMail{});
        assert(std::get<Answer::GetMail>(ans2).mail.empty());
        Letter l{"b", "a", "Hey, A!"};
        db.AddLetter(l);
        Answer::Any ans3 = query_processor(Query::GetMail{});
        std::vector<Letter> expect3{l};
        assert(std::get<Answer::GetMail>(ans3).mail == expect3);
    }

    void TestSendLetterProcessing() {

    }

    void TestAuthorizeProcessing() {

    }

    void TestTerminateProcessing() {

    }
}


void TestQueryProcessor() {
    TestGetMailProcessing();
    TestSendLetterProcessing();
    TestAuthorizeProcessing();
    TestTerminateProcessing();
    std::cerr << "TestQueryProcessor: OK" << std::endl;
}
