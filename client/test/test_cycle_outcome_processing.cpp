#include "test_cycle_outcome_processing.h"
#include "client/source/cycle_outcome_processing.h"


#include <iostream>
#include <cassert>
#include <sstream>


namespace {
    void Test() {
        SessionState sst;
        std::ostringstream output;

        auto process = [&sst, &output](Query::Any query, Answer::Any answer) {
            ProcessCycleOutcome(query, answer, sst, output);
        };
        process(Query::GetMail{}, Answer::GetMail{});

        Answer::GetMail ans = {{{"a", "Nikita", "c\nd\\e"},
                                 {"x", "Nikita", "z\nz\n"}}};
        process(Query::GetMail{}, ans);

        process(Query::SendLetter{{"a", "b", "c"}}, Answer::SendLetter{});

        process(Query::GetMail{}, Answer::Error{"Fatal error"});

        process(Query::Authorize{"Nikita", "correct password"},
                Answer::Authorize{"Nikita"});
        assert(sst.user_login == "Nikita");

        // Authorization failed
        process(Query::Authorize{"Nikita", "invalid password"},
                Answer::Authorize{});

        process(Query::Authorize{"Kot", "."}, Answer::Any{Answer::Authorize{"Kot"}});
        assert(sst.user_login == "Kot");

        process(Query::Terminate{}, Answer::Terminate{});
        assert(!sst.running);

        std::string expected_output = R"(Your mailbox is empty

From: a
c
d\e
---------------
From: x
z
z


Letter sent successfully

Server error: Fatal error

Authorized successfully

Wrong password

Authorized successfully

Session terminated

)";
        assert(output.str() == expected_output);
    }
}


void TestCycleOutcomeProcessing() {
    Test();
    std::cerr << "TestCycleOutcomeProcessing: OK" << std::endl;
}
