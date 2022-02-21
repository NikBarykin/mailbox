#include "test_query_processor.h"
#include "../source/server.h"
#include "../source/query_processor.h"
#include "common/test/testing_utility.h"

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

        std::vector<Letter> ls = {"b a x"_l, "c a apop"_l};
        db.AddLetters(ls.begin(), ls.end());

        Answer::Any ans3 = query_processor(Query::GetMail{});
        auto res3 = std::get<Answer::GetMail>(ans3).mail;
        assert(std::is_permutation(res3.begin(), res3.end(), ls.begin()));

        Answer::Any ans4 = query_processor(Query::GetMail{R"((from == "b" && body != "x") || from == "c")"});
        Answer::Any expected4 = Answer::GetMail{{ls[1]}};
        assert(ans4 == expected4);

        // Invalid filters
        Answer::Any ans5 = query_processor(Query::GetMail{"RANDOMTEXT"});
        assert(std::holds_alternative<Answer::Error>(ans5));

        Answer::Any ans6 = query_processor(Query::GetMail{"06.07.100 < body"});
        assert(std::holds_alternative<Answer::Error>(ans6));
    }

    void TestSendLetterProcessing() {
        SessionState session_state;
        Database db;
        QueryProcessor query_processor(db, session_state);

        Letter l1 = "a b 1"_l;

        // Not authorized
        Answer::Any ans1 = query_processor(Query::SendLetter{l1});
        assert(std::holds_alternative<Answer::Error>(ans1));

        session_state.user_id = *db.Authorize("a", "a");

        Answer::Any ans2 = query_processor(Query::SendLetter{l1});
        assert(std::holds_alternative<Answer::SendLetter>(ans2));
        assert(db.GetMail(db.GetId("b")) == std::vector<Letter>{l1});

        Letter l2 = "b a 2"_l;

        query_processor(Query::SendLetter{l2});
        assert(db.GetMail(*session_state.user_id) == std::vector<Letter>{l2});

        Letter l3 = "a b 3"_l;
        query_processor(Query::SendLetter{l3});
        std::vector<Letter> expect = {l1, l3};
        assert(db.GetMail(db.GetId("b")) == expect);
    }

    void TestAuthorizeProcessing() {
        Database db;
        SessionState sst;
        QueryProcessor query_processor(db, sst);


        Answer::Any ans1 = query_processor(Query::Authorize{"a", "a"});
        assert(std::get<Answer::Authorize>(ans1).authorized_login == "a");

        Answer::Any ans2 = query_processor(Query::Authorize{"a", "b"});
        assert(std::get<Answer::Authorize>(ans2).authorized_login.empty());
    }

    void TestTerminateProcessing() {
        Database db;
        SessionState sst;
        QueryProcessor query_processor(db, sst);

        Answer::Any ans1 = query_processor(Query::Terminate{});
        assert(std::holds_alternative<Answer::Terminate>(ans1));

        // Query in terminated session
        Answer::Any ans2 = query_processor(Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans2));
    }
}


void TestQueryProcessor() {
    TestGetMailProcessing();
    TestSendLetterProcessing();
    TestAuthorizeProcessing();
    TestTerminateProcessing();
    std::cerr << "TestQueryProcessor: OK" << std::endl;
}
