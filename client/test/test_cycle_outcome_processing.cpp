#include "test_cycle_outcome_processing.h"


#include <iostream>
#include <cassert>
#include <sstream>

#include "client/source/cycle_outcome_processing.h"
#include "common/test/testing_utility.h"


namespace {
    void Test() {
        SessionState sst;
        std::ostringstream output;

        auto process = [&sst, &output](Query::Any query, Answer::Any answer) {
            ProcessCycleOutcome(query, answer, sst, output);
        };
        process(Query::GetMail{}, Answer::GetMail{});

        Answer::GetMail ans = {{"a Nikita c\nd\\e"_l,
                                "x Nikita z\nz\n"_l}};
        process(Query::GetMail{}, ans);

        process(Query::SendLetter{"a b c"_l}, Answer::SendLetter{});

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

        std::ostringstream expected_output;
        expected_output << "No mail found\n\n"
                        << "1.\n"
                        << "Date: " << Date::CurrentDate().AsString() << '\n'
                        << "From: a\n"
                        << "c\nd\\e\n"
                        << "2.\n"
                        << "Date: " << Date::CurrentDate().AsString() << '\n'
                        << "From: x\n"
                        << "z\nz\n\n\n"
                        << "Letter sent successfully\n\n"
                        << "Server error: Fatal error\n\n"
                        << "Authorized successfully\n\n"
                        << "Wrong password\n\n"
                        << "Authorized successfully\n\n"
                        << "Session terminated\n\n";

        assert(output.str() == expected_output.str());
    }
}


void TestCycleOutcomeProcessing() {
    Test();
    std::cerr << "TestCycleOutcomeProcessing: OK" << std::endl;
}
