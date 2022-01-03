#include "test_query_processor.h"
#include "../source/server.h"
#include "../source/query_processor.h"
#include "general/test/testtools.h"

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
        SessionState session_state;
        Database db;
        QueryProcessor query_processor(db, session_state);

        Letter l1{"a", "b", "1"};

        // Not authorized
        Answer::Any ans1 = query_processor(Query::SendLetter{l1});
        assert(std::holds_alternative<Answer::Error>(ans1));

        session_state.user_id = *db.Authorize("a", "a");

        Answer::Any ans2 = query_processor(Query::SendLetter{l1});
        assert(std::holds_alternative<Answer::SendLetter>(ans2));
        assert(db.GetMail(db.GetId("b")) == std::vector<Letter>{l1});

        Letter l2{"b", "a", "2"};

        query_processor(Query::SendLetter{l2});
        assert(db.GetMail(*session_state.user_id) == std::vector<Letter>{l2});

        Letter l3{"a", "b", "3"};
        query_processor(Query::SendLetter{l3});
        std::vector<Letter> expect = {l1, l3};
        assert(db.GetMail(db.GetId("b")) == expect);
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
