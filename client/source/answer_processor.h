#pragma once


#include "general/source/answer.h"
#include "session.h"

#include <variant>
#include <iostream>


class AnswerProcessor {
private:
    SessionState& session_state_;
    std::ostream& output_;
public:
    AnswerProcessor(SessionState& session_state, std::ostream& output);

    void operator ()(Answer::GetMail answer);
    void operator ()(Answer::Authorize answer);
    void operator ()(Answer::Error answer);
    void operator ()(Answer::SendLetter answer);
    void operator ()(Answer::Terminate answer);

    void operator ()(Answer::AnsT answer);
};
