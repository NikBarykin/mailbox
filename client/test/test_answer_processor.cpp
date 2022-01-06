#include "test_answer_processor.h"
#include "client/source/answer_processor.h"


#include <iostream>
#include <cassert>
#include <sstream>


namespace {
    void Test() {
        SessionState sst;
        std::ostringstream output;
        AnswerProcessor answer_processor(sst, output);

        answer_processor(Answer::GetMail{});

        Answer::GetMail ans = {{{"a", "Nikita", "c\nd\\e"},
                                 {"x", "Nikita", "z\nz\n"}}};
        answer_processor(ans);

        answer_processor(Answer::SendLetter{});

        answer_processor(Answer::Error{"Fatal error"});

        answer_processor(Answer::Authorize{"Nikita"});
        assert(sst.user_login == "Nikita");

        // Authorization failed
        answer_processor(Answer::Authorize{});

        answer_processor(Answer::Any{Answer::Authorize{"Kot"}});
        assert(sst.user_login == "Kot");

        answer_processor(Answer::Terminate{});
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


void TestAnswerProcessor() {
    Test();
    std::cerr << "TestAnswerProcessor: OK" << std::endl;
}
